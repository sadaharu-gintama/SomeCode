// 1. in the header file, we need definitions from the following include header
#include <boost/config.hpp>

// 2. the following code must also be added
#if defined(MY_LIBRARY_LINK_DYNAMIC)
  #if defined(MY_LIBRARY_COMPILATION)
    #define MY_LIBRARY_API BOOST_SYMBOL_EXPORT
  #else
    #define MY_LIBRARY_API BOOST_SYMBOL_IMPORT
  #endif
#else
  #define MY_LIBRARY_API
#endif

// 3. now all the declaration must use the MY_LIBRARY_API macro
int MY_LIBRARY_API foo();
class MY_LIBRARY_API bar {
 public:
  /*...*/
  int meow() const;
};

// 4. exceptions must be declared with BOOST_SYMBOL_VISIBLE. otherwise they can be caught only using catch(...)
#include <stdexcept>
struct BOOST_SYMBOL_VISIBLE bar_exception : public std::exception {};
