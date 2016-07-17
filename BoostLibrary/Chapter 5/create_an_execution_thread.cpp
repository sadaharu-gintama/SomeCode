// Multithread
#include <algorithm>
#include <fstream>
#include <iterator>

//##############################################################################
void set_not_first_run();
bool is_first_run();

// Function that executes for a long time
void fill_file_with_data(char fill_char, std::size_t size, const char *filename) {
  std::ofstream ofs(filename);
  std::fill_n(std::ostreambuf_iterator<char>(ofs), size, fill_char);
  set_not_first_run();
}

// somewhere in thread that draws a user interface
if (is_first_run()) {
  // this will be executing for a long time during which user interface
  // will freeze
  fill_file_with_data(0, 8 * 1024 * 1024, "save_file.txt");
 }


//##############################################################################
// Boost thread library
#include <boost/thread.hpp>

// somewhere in thread that draws a user interface
if (is_first_run()) {
  boost::thread(boost::bind(&fill_file_with_data,
                            0,
                            8 * 1024 * 1024,
                            "save_file.txt")).detach();
}

//##############################################################################
// we want to make sure that the file was created and written before
// doing some other work
if (is_first_run()) {
  boost::thread t(boost::bind(&fill_file_with_data,
                              0,
                              8 * 1024 * 1024,
                              "save_file.txt"));
  // Do some work
  // ......
  // waiting for thread to finish
  t.join();
}

//##############################################################################
// small example for scoped_thread, raii wrapper around thread
#include <boost/thread/scoped_thread.hpp>

void some_func();
void example_with_raii() {
  boost::scoped_thread<boost::join_if_joinable> t (
    (boost::thread(&some_func)));
  // t will be joined at scope exit
}
