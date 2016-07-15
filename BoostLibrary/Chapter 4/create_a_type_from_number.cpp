// where we have a generic method for processing POD datatypes:
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_pod.hpp>

//generic implementation
template<class T>
T process(const T &val) {
  BOOST_STATIC_ASSERT((boost::is_pod<T>::value));
};

// we have the same function optimized for sizes 1, 4, and 8 bytes, how do we
// rewrite process function so that it can dispatch calls to optimized version?

// define generic and optimized version of process_impl function
#include <boost/mpl/int.hpp>
namespace detail {
  //Generic implementation
  template<class T, class Tag>
  T process_impl(const T &val, Tag /* ignore */) {
    //...
  };

  // 1 byte optimized implementation
  template<class T>
  T process_impl(const T &val, boost::mpl::int_<1> /* ignore*/) {
    // ...
  };

  // 4 byte optimized implementation
  template<class T>
  T process_impl(const T &val, boost::mpl::int_<4> /* ignore*/) {
    // ...
  };

  // 8 byte optimized implementation
  template<class T>
  T process_impl(const T &val, boost::mpl::int_<8> /* ignore*/) {
    // ...
  };
}; // namespace detail

// now we are ready to write process functioin
template<class T>
T process(const T &val) {
  BOOST_STATIC_ASSERT((boost::is_pod<T>::value));
  return detail::process_impl(val, boost::mpl::int_<sizeof(T)>());
};
