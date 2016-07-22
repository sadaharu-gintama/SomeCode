#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>

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
};


int g_val = 0;
void func_test() {
  ++g_val;
  if (g_val == 3) {
    throw std::logic_error("just checking");
  }
  boost::this_thread::interruption_point();
  if (g_val == 10) {
    throw boost::thread_interrupted();
  }

  if (g_val == 90) {
    tasks_processor::get().stop();
  }
}

int main() {
  static const std::size_t tasks_count = 100;
  BOOST_STATIC_ASSERT(tasks_count > 90);
  for (std::size_t i = 0; i < tasks_count; ++i) {
    tasks_processor::get().push_task(&func_test);
  }

  tasks_processor::get().push_task(boost::bind(std::plus<int>(), 2, 2));

  assert(g_val == 0);

  tasks_processor::get().start();
  assert(g_val == 90);
  return 0;
}
