// template functions
template <class T>
T process_data(const T &v1, const T &v2, const T &v3);

// now that we write code using process_data function, we use an optimized
// process_data version for types that do have an operator += function
template <class T>
T process_data_plus_assign(const T &v1, const T &v2, const T &v3);

// but we do not want to change the already written code
// whenever it is possible, we want to force the compiler to automatically
// use optimized function in place of the default one
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_plus_assign.hpp>

// now we disable default implementation for types with plus assign operator
template <class T>
typename boost::disable_if_c<boost::has_plus_assign<T>::value, T>::type
process_data(const T &v1, const T &v2, const T &v3);

// enable optimized version for types with plus assign operator
template<class T>
typename boost::enable_if_c<boost::has_plus_assign<T>::value, T>::type
process_data(const T &v1, const T &v2, const T &v3) {
  return process_data_plus_assign(v1, v2, v3);
}

// now user won't feel the difference
// but the optimized verison will be used wherever possible
int main() {
  int i = 1;
  // optimized version
  process_data(i, i, i);

  // generic verison
  // explicitly specifying template parameter
  process_data<const char*>("testing", "exmaple", "function");

  return 0;
}
 
