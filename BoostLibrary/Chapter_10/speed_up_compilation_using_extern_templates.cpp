// 1. include header
#include <boost/config.hpp>
// 2. include header that contains a template class whose instantiation count we wish to reduce
#include <boost/variant.hpp>
#include <boost/blank.hpp>
#include <string>

// 3. code for compilers with support for C++11 extern templates
#ifndef BOOST_NO_CXX11_EXTERN_TEMPLATE
extern template class boost::variant<boost::blank, int, std::string, double>;
#endif

// 4. now we need to add the following code to source file where we wish the tepmlate to be instantiated.
//#include "header.hpp". the header store step 2,3
#ifndef BOOST_NO_CXX11_EXTERN_TEMPLATE
template class boost::variant<boost::blank, int, std::string, double>;
#endif
