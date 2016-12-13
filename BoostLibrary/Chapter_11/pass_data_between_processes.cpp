#include <iostream>

// 1. we will need to include the header
#include <boost/interprocess/managed_shared_memory.hpp>

// 2. typedef and a check
#include <boost/atomic.hpp>

typedef boost::atomic<int> atomic_t;
#if (BOOST_ATOMIC_INT_LOCK_FREE != 2)
#error "This code requires lock-free boost::atomic<int>"
#endif

// 3. Create or get a shared segment of memory
boost::interprocess::managed_shared_memory
segment(boost::interprocess::open_or_create, "shm-cache", 1024);

// 4. get or construct an atomic variable
atomic_t &atomic = *segment.find_or_construct<atomic_t>("shm-counter")(0);

// 5. work with the atomic variable in the usual way
std::cout << "I have index" << ++atomic << "\nPress Any Key...";
std::cin.get();

// 6. destroy the atomic variable
int snapshot = --atomic;
if (!snapshot) {
  segment.destroy<atomic_t>("shm-counter");
  boost::interprocess::shared_memory_object::remove("shm-cache");
}
