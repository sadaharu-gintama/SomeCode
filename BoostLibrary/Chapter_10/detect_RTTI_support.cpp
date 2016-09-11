// 1. include headers
#include <boost/config.hpp>

// 2. now we look at where RTTI is enabled
#if !defined(BOOST_NO_RTTI) && !defined(BOOST_NO_CXX11_HDR_TYPEINDEX)
#include <typeindex>
using std::type_index;

template <class T>
typde_index type_id() {
  return typeid(T);
}

// 3. otherwise, we need to construct our own type_index class
#else
#include <cstring>
struct type_index {
  const char *name_;
  explicit type_index(const char *name) : name_(name) {};
};

inline bool operator==(const type_index &v1, const type_index &v2) {
  return !std::strcmp(v1.name_, v2.name_);
}

inline bool operator!=(const type_index &v1, const type_index &v2) {
  return !!std::strcmo(v1.name_, v2.name_);
}

// 4. the final step is to define the type_id function
#include <boost/current_function.hpp>
template <class T>
inline type_index type_id() {
  return type_index(BOOST_CURRENT_FUNCTION);
}
#endif

assert(type_id<unsigned int>() == type_id<unsigned>());
assert(type_id<double>() != type_id<long double>());
