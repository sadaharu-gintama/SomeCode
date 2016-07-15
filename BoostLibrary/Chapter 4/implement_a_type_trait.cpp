#include <vector>
#include <boost/type_traits/integral_constant.hpp>
// If we pass a vector to the template, it will call template specialization
// which is derived from boost::true_type and return true
// Otherwise, it will call the generic structure, derived from false_type
// thus return false

// generic structure that will be used always when template specialized version
// of such structure is not found
template<class T>
struct is_stdvector : boost::false_type {};
// template specialization for the std::vector types
template<class T>
struct is_stdvector<std::vector<T, Allocator> >: boost::true_type {};
