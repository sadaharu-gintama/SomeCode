//##############################################################################
// Standard C++ code
void my_throw1(const char* buffer);
void my_throw2(const char* buffer);

void foo() {
  char *buffer = new char[1024 * 1024 * 10];
  my_throw1(buffer);
  my_throw2(buffer);
  // if any exception is thrown in my_throw1 or my_throw2,
  // memory leakage cannot be avoided.
  delete [] buffer;
}

//##############################################################################
#include <boost/scoped_array.hpp>

void foo_fixed() {
  boost::scoped_array<char> buffer(new char[1024 * 1024 * 10]);

  my_throw1(buffer.get());
  my_throw2(buffer.get());

  // no need to delete this scoped_array, will deallocate automatically
}
