// 5. library source files must include the header files
#define MY_LIBRARY_COMPILATION
#include "portable_way_to_export_and_import_functions_and_class.hpp"

// 6. Definitions of methods also must be in the source files of the library
int MY_LIBRARY_API foo() {
  // Implementation
  // ...
  return 0;
}

int bar::meow() const {
  throw bar_exception();
}

// 7. now we can use the library
#include <cassert>

int main() {
  assert(foo() == 0);
  bar b;
  try {
    b.meow();
    assert(false);
  }
  catch (const bar_exception &) {}
}
