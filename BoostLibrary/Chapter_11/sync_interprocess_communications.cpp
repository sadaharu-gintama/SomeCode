// 1. headers
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

// 2. we need to define our struct
struct task_struct {
  // ...
};

// 3. let's start writing the work_queue class
class work_queue {
public:
  typedef task_struct task_type;
  typedef boost::interprocess::managed_shared_memory managed_shared_memory_t;
  typedef boost::interprocess::allocator<task_type,
                                         managed_shared_memory_t::
                                         segment_manager> allocator_t;

  // 4. write the members of work_queue as follows:
private:
  managed_shared_memory_t segment_;
  const allocator_t allocator_;

  typedef boost::interprocess::deque<task_type, allocator_t> deque_t;
  typedef boost::interprocess::interprocess_mutex mutex_t;
  typedef boost::interprocess::interprocess_condition condition_t;
  typedef boost::interprocess::scoped_lock<mutex_t> scoped_lock_t;

  deque_t &tasks_;
  mutex_t &mutex_;
  boost::interprocess::interprocess_condition &cond_;
  // 5. initialization of members should look like this
public:
  explicit work_queue() : segment_(boost::interprocess::open_or_create,
                                   "work-queue",
                                   1024 * 1024 * 64),
                          allocator_(segment_.get_segment_manager()),
                          tasks_(*segment_.find_or_construct<deque_t>
                                 ("work-queue:deque")(allocator_)),
                          mutex_(*segment_.find_or_construct<mutex_t>
                                 ("work-queue:mutex")()),
                          cond_(*segment_.find_or_construct<condition_t>
                                ("work-queue:condition")()) {};
  // 6. we need to make some minor changes to the member functions of work_queue
  boost::optional<task_type> try_pop_task() {
    boost::optional<task_type> ret;
    scoped_lock_t lock(mutex_);
    if (!tasks_.empty()) {
      ret = tasks_.front();
      tasks_pop_front();
    }
    return ret;
  }

};
