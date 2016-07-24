#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>

namespace detail {
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

  class tcp_listener : public boost::enable_shared_from_this<tcp_listener> {
    typedef boost::asio::ip::tcp::acceptor acceptor_t;
    acceptor_t acceptor_;
    boost::function<void(tcp_connection_ptr)> func_;
  public:
    template<class Functor>
    tcp_listener(boost::asio::io_service &io_service,
                 unsigned short port,
                 const Functor &task_unwrapped) :
      acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(), port)),
      func_(task_unwrapped) {};
    void push_task() {
      if (!acceptor_.is_open()) { return; }
      typedef boost::asio::ip::tcp::socket socket_t;
      boost::shared_ptr<socket_t> socket =
        boost::make_shared<socket_t>(boost::ref(acceptor_.get_io_service()));
      acceptor_.async_accept(*acceptor_, boost::bind(
                             &tcp_listener::handle_accept,
                             this->shared_from_this(),
                             tcp_connection_ptr(socket),
                             boost::asio::placeholder::error));
    }
    void stop() { acceptor_.close(); }
  private:
    void handle_accept(const tcp_connection_ptr &new_connection,
                       const boost::system::error_code &error) {
      push_task();
      if (!error) {
        make_task_wrapped(boost::bind(func_, new_connection)) ();
      }
      else {
        std::cerr << error << "\n";
      }
    }
  };

} // namespace detail

class tasks_processor : private boost::noncopyable {
  boost::asio::io_service ios_;
  boost::asio::io_service::work work_;
  tasks_processor() : ios_(), work_(ios_) {};
public:
  typedef std::map<unsigned short, boost::shared_ptr<detail::tcp_listener>> \
    listeners_map_t;
  listeners_map_t listeners_;

  static tasks_processor & get();
  //push_task method
  template<class T>
  inline void push_task(const T &task_unwrapped) {
    ios_.post(detail::make_task_wrapped(task_unwrapped));
  }
  //start and stop task execution loop
  void start() { ios_.run(); }
  void stop() { ios_.stop(); }

  // add a listener
  template<class Functor>
  void add_listener(unsigned short port_num, const Functor &f) {
    listeners_map_t::const_iterator it = listeners_.find(port_num);
    if (it != listeners_.end()) {
      throw std::logic_error("Such listener for port " +
                             boost::lexical_cast<std::string>(port_num) +
                             " has already created.");
    }

    listeners_[port_num] = boost::make_shared<detail::tcp_listener>(
                           boost::ref(ios_), port_num, f);
    listeners_[port_num]->push_task();
  }

  // remove a listener
  void remove_listener(unsigned short port_num) {
    listeners_map_t::iterator it = listeners_.find(port_num);
    if (it == listeners_.end()) {
      throw std::logic_error("No listener for port " +
                             boost::lexical_cast<std::string>(port_num) +
                             " creaded.");
    }
    (*it).second->stop();
    listeners_.erase(it);
  }
};
