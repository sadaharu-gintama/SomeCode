#include <boost/scoped_ptr.hpp>
class foo_class {};
bool some_function1(foo_class *ptr);

// with this scoped_ptr, if there is any exception thrown in some_function1,
// the pointer is still safe. no memory leakage and the code is clearer.
// is similar to std::auto_ptr

bool foo3() {
  boost::scoped_ptr<foo_class> p(new foo_class());
  bool something_happened = some_function1(p.get());
  if (something_happened) {
    return false;
  }
  return true;
}
