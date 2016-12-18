#include <stdexcept>
struct foo {
  int val_;
  operator int() const;
  bool is_not_null() const;
  void throws() const; // throws std::logic_error
};


// 1. define the macro and include header
#define BOOST_TEST_MODULE test_module_name
#include <boost/test/unit_test.hpp>

// 2. each set of tests must be written in the test case
BOOST_AUTO_TEST_CASE(test_no_1) {
  // 3. checking some function for the true result is done as follows
  foo f1 = {1}, f2 = {2};
  BOOST_CHECK(f1.is_not_null());
  // 4. check for nonequality is implemented in the following way
  BOOST_CHECK_NE(f1, f2);
  // 5. check for an exception being thrown
  BOOST_CHECK_THROW(f1.throws(), std::logic_error);
}
