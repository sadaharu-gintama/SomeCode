// we have threads that post tasks and threads that execute posted tasks.
// we need to design a class that can be safely used by both types of threads/

#include <deque>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

class work_queue {
public:
  typedef boost::function<void()> task_type;
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

// here is how to use work_queue class
#include <boost/thread/thread.hpp>

work_queue g_queue;

void do_nothing() {};

const std::size_t tests_tasks_count = 3000;

void pusher() {
  for (std::size_t i = 0; i < tests_tasks_count; ++i) {
    // Adding task to do nothing
    g_queue.push_task(&do_nothing);
  }
}

void popper_sync() {
  for (std::size_t i = 0; i < tests_tasks_count; ++i) {
    g_queue.pop_task(); // getting task
    (); // executing tasks
  }
}

int main() {
  boost::thread pop_sync1(&popper_sync);
  boost::thread pop_sync2(&popper_sync);
  boost::thread pop_sync3(&popper_sync);

  boost::thread push1(&pusher);
  boost::thread push2(&pusher);
  boost::thread push3(&pusher);

  // wait for all the tasks to pop
  pop_sync1.join();
  pop_sync2.join();
  pop_sync3.join();

  push1.join();
  push2.join();
  push3.join();

  // assert that no tasks remained and falling through without blocking
  assert(!g_queue.try_pop_task());

  g_queue.push_task(&do_nothing);

  // assert that there is a task, and falling through without blocking
  assert(!g_queue.try_pop_task());

  return 0;
}
