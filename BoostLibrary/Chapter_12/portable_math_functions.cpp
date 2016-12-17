// 1. following headers
#include <boost/math/special_functions.hpp>
#include <cassert>

// 2. asserting for inifinity and NaN can be done like this
template <class T>
void check_float_inputs(T value) {
  assert(!boost::math::isinf(value));
  assert(!boost::math::isnan(value));

// 3. use the following code to change the sign
  if (boost::math::signbit(value)) {
    value = boost::math::changesign(value);
  }
}
