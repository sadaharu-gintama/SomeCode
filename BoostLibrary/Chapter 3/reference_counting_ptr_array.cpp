//##############################################################################
// problematic code
#include <cstring>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

void do_process(const char *data, std::size_t size);
void do_process_in_background(const char *data, std::size_t size) {
  char *data_cpy = new char[size];
  std::memcpy(data_cpy, data, size);

  boost::thread(boost::bind(&do_process, data_cpy, size)).detach();
  // cannot delete[] data_cpy, because do_process1 or do_process2 may still
  // work on this
}

//##############################################################################
// Solution 1: shared_array, don't need to pass size in thread
#include <boost/shared_array.hpp>
// add a wrapper around the do_process above
void do_process1(const boost::shared_array<char> &data, std::size_t size) {
  do_process(data.get(), size);
}

void do_process_in_background_v1(const char *data, std::size_t size) {
  boost::shared_array<char> data_cpy(new char[size]);
  std::memcpy(data_cpy.get(), data, size);

  boost::thread(boost::bind(&do_process1, data_cpy)).detach();

  // no need to call destructor, the pointer is a smart pointer
}

//##############################################################################
// Solution 2: shared_ptr can also handle it
// this solution and the solution below need to pass size in thread
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

void do_process_shared_ptr(const boost::shared_ptr<char[]> &data,
                           std::size_t size) {
  do_process(data.get(), size);
}

void do_process_in_background_v2(const char *data, std::size_t size) {
  boost::shared_ptr<char[]> data_cpy = boost::make_shared<char[]>(size);
  std::memcpy(data_cpy.get(), data, size);
  boost::thread(boost::bind(&do_process_shared_ptr, data_cpy, size)).detach();
  // also no need to call destructor
}

//##############################################################################
// Solution 3: also shared_ptr, but a little variation
void do_process_shared_ptr2(const boost::shared_ptr<char> &data,
                            std::size_t size) {
  do_process(data.get(), size);
}

void do_process_in_background_v3(const char *data, std::size_t size) {
  boost::shared_ptr<char> data_cpy(new char[size],
                                   boost::checked_array_deleter<char>());
  std::memcpy(data_cpy.get(), data, size);
  boost::thread(boost::bind(&do_process_shared_ptr2, data_cpy, size)).detach();
  // also no need to call destructor
}
