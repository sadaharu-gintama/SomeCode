// 1. we need to include headers containing the boost version
#include <boost/lexical_cast.hpp>
#include <boost/version.hpp>

// 2. we will use the new feature of boost.lexical_cast if possible:
#if (BOOST_VERSION >= 105200)
int to_int(const char *str, std::size_t length) {
  return boost::lexical_cast<int>(str, length);
}
#else
int to_int(const char *str, std::size_t length) {
  return boost::lexical_cast<int>(std::string(str, length));
}
#endif
