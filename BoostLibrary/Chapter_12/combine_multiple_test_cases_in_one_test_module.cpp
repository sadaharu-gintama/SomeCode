// 1. of all the headers foo.h in main.cpp from the previous recipe
#define BOOST_TEST_MODULE test_module_name
#include <boost/test/unit_test.hpp>

// 2. test cases into two different source file
// in source file one
#include <boost/test/unit_test.hpp>
#include "foo.h"
BOOST_AUTO_TEST_CASE(test_no_1) {
  // ......
}

// in source file two
#include <boost/test/unit_test.hpp>
#include "foo.h"
BOOST_AUTO_TEST_CASE(test_no_2) {
  // ......
}
