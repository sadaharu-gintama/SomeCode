#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/function.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

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

  // make_timer_task
  template<class Time, class Functor>
  inline timer_task<Functor> make_timer_task(boost::asio::io_service &ios,
                                             const Time &duration_or_time,
                                             const Functor &task_unwrapped) {
    return timer_task<Functor>(ios, duration_or_time, task_unwrapped);
  }
} // namespace detail

class work_queue {
public:
  typedef boost::function<void()> task_type;
  typedef boost::asio::deadline_timer::duration_type duration_type;
  template<class Functor>
  void run_after(duration_type duration, const Functor &f) {
    detail::make_timer_task(ios_, duration, f).push_task;
  }
  // put task in queue
  void push_task(const task_type &task) {
    boost::unique_lock<boost::mutex> lock(tasks_mutex_);
    tasks_.push_back(task);
    lock.unlock();
    cond_.notify_one();
  }
  // non-blocking function for getting a pushed task or empty task
  task_type try_pop_task() {
    task_type ret;
    boost::lock_guard<boost::mutex> lock(tasks_mutex_);
    if (!tasks_.empty()) {
      ret = tasks_.front();
      tasks_.pop_front();
    }
    return ret;
  }
  // blocking function for getting a pushed task or for blocking while the
  // task is pushed by another thread
  task_type pop_task() {
    boost::unique_lock<boost::mutex> lock(tasks_mutex_);
    while (tasks_.empty()) {
      cond_.wait(lock);
    }
    task_type ret = tasks_.front();
    tasks_.pop_front();
    return ret;
  }
private:
  std::deque<task_type> tasks_;
  boost::mutex tasks_mutex_;
  boost::condition_variable cond_;
};

class tasks_processor : private boost::noncopyable {
  boost::asio::io_service ios_;
  boost::asio::io_service::work work_;
  tasks_processor() : ios_(), work_(ios_) {};
  typedef boost::asio::deadline_timer::time_type time_type;
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
  // running task at some time
  template<class Functor>
  void run_at(time_type time, const Functor &f) {
    detail::make_timer_task(ios_, time, f).push_task();
  }
};


namespace detail {
  typedef boost::asio::deadline_timer::duration_type duration_type;
  template <class Functor>
  struct timer_task : public task_wrapped<Functor> {
  private:
    typedef task_wrapped<Functor> base_t;
    boost::shared_ptr<boost::asio::deadline_timer> timer_;
  public:
    template<class Time>
    explicit timer_task(boost::asio::io_service &ios,
                        const Time & duration_or_time,
                        const Functor &task_unwrapped) :
      base_t(task_unwrapped),
      timer_(boost::make_shared<boost::asio::deadline_timer>(
                   boost:ref(ios), duration_or_time)) {};
    void push_task() const {
      timer_->async_wait(*this);
    }

    void operator()(const boost::system::error_code &error) const {
      if (!error) {
        base_t::operator()();
      }
      else {
        std::cerr << error << "\n";
      }
    }
  };
}
