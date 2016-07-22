#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
class tcp_connection_ptr {
  boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
public:
  explicit tcp_connection_ptr(
    boost::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket) {};
  explicit tcp_connection_ptr(
    boost::asio::io_service &ios,
    const boost::asio::ip::tcp::endpoint &endpoint):
    socket_(boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(ios))) {
    socket_->connect(endpoint);
  }

  template<class Functor>
  void async_read(const boost::asio::mutable_buffers_1 &buf,
                  const Functor &f,
                  std::size_t at_least_bytes) const {
    boost::asio::async_read(*socket_, buf, boost::asio::transfer_at_least(
         at_least_bytes), f);
  }

  template<class Functor>
  void async_write(const boost::asio::const_buffers_1 &buf,
                   const Functor &f) const {
    boost::asio::async_write(*socket_, buf, f);
  }

  template<class Functor>
  void async_write(const boost::asio::mutable_buffers_1 &buf,
                   const Functor &f) const {
    boost::asio::async_write(*socket_, buf, f);
  }

  void shutdown() const {
    socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket_->close();
  }
};

namespace detail {
  template<class T>
  struct task_wrapped {
  private:
    T task_unwrapped_;

  public:
    explicit task_wrapped(const T &task_unwrapped) :
      task_unwrapped_(task_unwrapped) {};

    void operator() () const {
      //resetting interruption
      try {
        boost::this_thread::interruption_point();
      } catch (const boost::thread_interrupted &) {};

      try {
        // Executing task
        task_unwrapped_();
      }
      catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
      }
      catch (const boost::thread_interrupted &) {
        std::cerr << "Thread interrupted\n" << std::endl;
      }
      catch (...) {
        std::cerr << "Unknown exception\n" << std::endl;
      }
    }
  };

  // each of use, we will create a function that produces task_wrapped from
  // the user's functor
  template<class T>
  task_wrapped<T> make_task_wrapped(const T &task_unwrapped) {
    return task_wrapped<T>(task_unwrapped);
  }
} // namespace detail

class tasks_processor : private boost::noncopyable {
  boost::asio::io_service ios_;
  boost::asio::io_service::work work_;
  tasks_processor() : ios_(), work_(ios_) {};
public:
  static tasks_processor & get();
  //push_task method
  template<class T>
  inline void push_task(const T &task_unwrapped) {
    ios_.post(detail::make_task_wrapped(task_unwrapped));
  }
  //start and stop task execution loop
  void start() { ios_.run(); }
  void stop() { ios_.stop(); }
  // create sockets to endpoint
  tcp_connection_ptr create_connection(const char *addr,
                                       unsigned short port_num) {
    return tcp_connection_ptr(ios_, boost::asio::ip::tcp::endpoint(
      boost::asio::ip::address_v4::from_string(addr), port_num));
  }
};

//##############################################################################
// Here is how user can use the classes to send data
const unsigned short g_port_num = 65001;

void send_auth_task() {
  tcp_connection_ptr soc = tasks_processor::get().create_connection(
     "127.0.0.1", g_port_num);
  boost::shared_ptr<std::string> data = boost::make_shared<std::string>
    ("auth_name");
  soc.async_write(boost::asio::buffer(*data),
                  boost::bind(&receive_auth_task,
                              boost::asio::placeholders::error,
                              soc, ata));
}

void receive_auth_task(const boost::system::error_code &err,
                       const tcp_connection_ptr &soc,
                       const boost::shared_ptr<std::string> &data) {
  if (err) {
    std::cerr << "Receive_auth_task: Client error on receive: " <<
      err.message() << "\n";
  }

  soc.async_read(boost::asio::buffer(&(*data)[0], data->size()),
                 boost::bind(&finish_scoket_auth_task,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred,
                             soc, data), 1);
}

bool g_authed = false;

void finish_scoket_auth_task(const boost::system::error_code &err,
                             std::size_t bytes_transferred,
                             const tcp_connection_ptr &soc,
                             const boost::shared_ptr<std::string> &data) {
  if (err && err != boost::asio::error::eof) {
    std::cerr << "finish_socket_auth_task: Client error on receive: "
              << err.message() << "\n";
    assert(false);
  }

  if (bytes_transferred != 2) {
    std::cerr << "finish_socket_auth_task: wrong bytes count\n";
    assert(false);
  }

  data->resize(bytes_transferred);
  if (*data != "OK") {
    std::cerr << "finish_socket_auth_task: wrong response: " << *data << "\n";
    assert(false);
  }

  g_authed = true;
  soc.shutdown();
  task_processor::get().stop();
}
