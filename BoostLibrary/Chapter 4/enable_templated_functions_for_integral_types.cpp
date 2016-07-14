// Generic implementation
template<class T>
class data_processor {
  double process(const T &v1, const T &v2, const T &v3);
};

//Two optimized versions of that class after execution
// Integral type
template<class T>
class data_processor {
  typedef int fast_int_t;
  double process(fast_int_t v1, fast_int_t v2, fast_int_t v3);
};

// float type
template<class T>
class data_processor {
  double process(double v1, double v2, double v3);
};

// Now the question, how to make the compiler to automatically choose the
// correct class for a specified type, arises.
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>

// Generic implementation
template<class T, class Enable = void>
class data_processor {
  ...
};

// Optimized versions
// integral type
template<class T>
class data_processor<T, typename boost::enable_if_c<
                          boost::is_integral<T>::value
                          >::type> {
  ...
};

// float type
template<class T>
class data_processor<T, typename boost::enable_if_c<
                          boost::is_float<T>::value
                          >::type> {
  ...
};

// now the compiler will automatically choose the correct class
template<class T>
double example_function(T v1, T v2, T v3) {
  data_processor<T> proc;
  return proc.process(v1, v2, v3);
}

int main () {
  // integral optimized version will be called
  example_function(1, 2, 3);
  short s = 0;
  example_function(s, s, s);

  // real type version will be called
  example_function(1.0, 2.0, 3.0);

  // Generic version will be called
  example_function("data", "generic", "processing");
}

