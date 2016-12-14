// 1. boost.corounte library
#include <boost/coroutine/coroutine.hpp>

// 2. make a corounte type with the required signature
typedef boost::coroutine::coroutine<
  std::string &(std::suze_t max_characters_to_process)> corout_t;

// 3. make a coroutine
void coroutine_task(corout_t::caller_type &caller_type);

int main() {
  corout_t coroutine(coroutine_task);

  // 4. now we can execute the subprogram while waiting for an event in the main
  // Doing some work
  // ...
  while (!spinlock.try_loc()) {
    // we may do some useful work before attempting to lock a spinlock
    coroutine(10); // small delays
  }
  // spinlock is locked

  // ...
  while(!port.block_ready()) {
    // we may do some useful work, before attempting to get block of data
    coroutine(300); // bigger delays
    std::string *s = coroutine.get();
  }
}

void coroutine_task(corout_t::caller_type &caller) {
  std::string result;

  // returning back to main program
  caller(result);

  while(1) {
    std::siz_t max_characters_to_process = caller.get();
    // do process some characters
    // ...

    // returning result, switching back
    // to main program
    caller(result);
  }
}
