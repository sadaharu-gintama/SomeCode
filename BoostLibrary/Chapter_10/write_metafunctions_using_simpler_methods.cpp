// 1. include header
#include <boost/config.hpp>
// 2. now we will work with constexpr
#if !defined(BOOST_NO_CXX11__CONSTEXPR) && !defined(BOOST_NO_CXX11_HDR_ARRAY)
template <class T>
constexpr int get_size(const T &val) {
  return val.size() * sizeof(typename T::value_type);
}
// 3. print an error if C++11 features are missing
#else
#error "this code requires C++11 constexpre and std::array"
#endif

// 4. now we are free to write code like this
std::array<short, 5> arr;
assert(get_size(arr) == 5 * sizeof(short));

unsigned char data[get_size(arr)];

