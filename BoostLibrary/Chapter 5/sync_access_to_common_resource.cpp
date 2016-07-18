// C++ traditional way
#include <cassert>
#include <cstddef>

//include boost thread
#include <boost/thread/thread.hpp>

int shared_i = 0;

void do_inc() {
  for (std::size_t i = 0; i < 30000; ++i) {
    // do some work
    // ......
    const int i_snapshot = ++ shared_i;
    // do some work with i_snapshot
    // ......
  }
}

void do_dec() {
  for (std::size_t i = 0; i < 30000; ++i) {
    // do some work
    // ......
    const int i_snapshot = --shared_i;
    // do some work with i_snapshot
    // ......
  }
}

void run() {
  boost::thread t1(&do_inc);
  boost::thread t2(&do_dec);

  t1.join();
  t2.join();

  // assert(shared_i == 0); // Ooops
  std::cout << "shared_i == " << shared_i << std::endl;
}

int main() {
  run();
  return 0;
}

//##############################################################################
// we need to change the code so that only one thread modify the shared_i
// variable at a single moment of time
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

int shared_i = 0;
boost::mutex i_mutex;

void do_inc() {
  for (std::size_t i = 0; i < 30000; ++i) {
    // do some work
    // ......
    int i_snapshot;
    { // critical section begins
      boost::lock_guard<boost::mutex> lock(i_mutex);
      i_snapshot = ++ shared_i;
    } // critical section ends
  }
}

void do_dec() {
  for (std::size_t i = 0; i < 30000; ++i) {
    // do some work
    // ......
    int i_snapshot;
    {
      boost::lock_guard<boost::mutex> lock(i_mutex);
      i_snapshot = -- shared_i;
    }
    // do some work with i_snapshot
    // ......
  }
}
