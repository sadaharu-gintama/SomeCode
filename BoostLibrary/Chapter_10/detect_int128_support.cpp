// 1. single header
#include <boost/config.hpp>

// 2. detect int 128 support
#ifdef BOOST_HAS_INT128

// 3. add some type defs and implement the methods as follows
typdef boost::int128_type int_t;
typdef boost::uint128_type uint_t;

inline int_t mul(int_t v1, int_t v2, int_t v3) {
  return v1 * v2 * v3;
}

// 4. for complier do not support int128 type, we may require support of int64
#else // BOOST_NO_LONG_LONG
#ifdef BOOST_NO_LONG_LONG
#error "this code requires at least int64_t support"
#endif
// 5. now we need to provide implementation for compilers without int128
struct int_t {boost::long_long_type hi, lo;};
struct uint_t {boost::ulong_long_type hi, low;};
inline int_t mul(int_t v1, int_t v2, int_t v3) {
  return v1 * v2 * v3;
}
#endif // BOOST_NO_LONG_LONG
