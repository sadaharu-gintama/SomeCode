#include <cstring>
#include <boost/array.hpp>
// Identify thos problems in compile time instead of run time
// size of buffer is not checked, it may overflow
// function can be used with non-plain old data types, which would lead to
// incorrect behavior
template <class T, std::size_t BufSizeV>
void serialize(const T &value, boost::array<unsigned char, BufSizeV> &buffer) {
  std::memcpy(&buffer[0], &value, sizeof(value));
}

// use boost.StaticAssert and boost.TypeTraits to correct
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

template<class T, std::size_t BufSizeV>
void serialize(const T &value, boost::array<unsigned char, BufSizeV> &buffer) {
  BOOST_STATIC_ASSERT(BufSizeV >= sizeof(value));
  BOOST_STATIC_ASSERT(boost::is_pod<T>::value);
  std::memcpy(&buffer[0], &value, sizeof(value));
}

// more examples
BOOST_STATIC_ASSERT(3 >= 1);

struct some_struct {enum enum_t { value = 1}; };
BOOST_STATIC_ASSERT(some_struct::value);

template<class T1, class T2>
struct some_templated_struct {
  enum enum_t {value = (sizeof(T1) == sizeof(T2))};
};
// if BOOST_STATIC_ASSERT macro's assert expression has a comma sign in it,
// we must wrap the whole expression in additional brackets.
BOOST_STATIC_ASSERT((some_templated_struct<int, unsigned int>::value));

//Boost.TypeTraits
#include <iostream>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

template <class T1, class T2>
void type_traits_example(T1 &v1, T2 &v2) {
  // check if it's unsigned
  std::cout << boost::is_unsigned<T1>::value;
  // check if the types are exactly the same
  std::cout <<boost::is_same<T1, T2>::value;

  // remove the const modifier from type of T1
  // const int => int
  // int => int
  // int const volatile => int volatile
  // const int& => const int&
  typedef typename boost::remove_const<T1>::type t1_nonconst_t;
}

// BOOST_STATIC_ASSERT_MSG that will output an error message in compiler log
template<class T, std::size_t BufSizeV>
void serialize2(const T &value, boost::array<unsigned char, BufSizeV> &buf) {
  BOOST_STATIC_ASSERT_MSG(boost::is_pod<T>::value,
                          "This function may be used only by POD type");
  BOOST_STATIC_ASSERT_MSG(BufSizeV >= sizeof(value), "Cannot fit into value");
  std::memcpy(&buf[0], &value, sizeof(value));
}
