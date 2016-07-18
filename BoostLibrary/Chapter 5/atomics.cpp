// previously we do two system calls just to get a value from an integer
//{
//boost::lock_guard<boost::mutex> lock(i_mutex);
//i_snapshot = ++shared_i;
//}

// we can make it better for using atomic
#include <cassert>
#include <cstddef>
#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>

boost::atomic<int> shared_i(0);

void do_inc() {
  for (std::size_t i = 0; i < 30000; ++i) {
    // do some work
    // ......
    const int i_snapshot = ++shared_i;
    // do some other work
    // ......
  }
}

void do_dec() {
  for (std::size_t i = 0; i < 30000; ++i) {
    // do some work
    // ......
    const int i_snapshot = --shared_i;
    // do some other work
    // ......
  }
}

int main() {
  boost::thread t1(&do_inc);
  boost::thread t2(&do_dec);

  t1.join();
  t2.join();

  assert(shared_i == 0);
  std::cout << "shared_i == " << shared_i << std::endl;
}

// boost.atomic can work only with POD type
// you can evaluate atomic behavior using boost::mutex
// the atomic type won't use boost::mutex if the type-specific macro is set
// to be 2
#include <boost/static_assert.hpp>
BOOST_STATIC_ASSERT(BOOST_ATOMIC_INT_LOCK_FREE == 2);
// the boost::atomic<T>::is_lock_free function depends on runtime
// so it's not good for compile-time check but may provide a more
// readable syntax when the runtime check is enough
assert(shared_i.is_lock_free());
