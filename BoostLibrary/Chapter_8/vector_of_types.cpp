#include <string>
#include <boost/mpl/vector.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/max_element.hpp>

// 1. pack all the types in one of the boost.mpl types container
template<
  class T0, class T1, class T2, class T3, class T4, class T5, class T6,
  class T7, class T8, class T9>
struct variant {
  typedef boost::mpl::vector<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> types;
};
// 2. make our example less abstract and see how it will work if we specify type
struct declared {unsigned char data[4096]; };

struct non_defined;

typedef variant<volatile int,
                const int,
                const long,
                declared,
                non_defined,
                std::string>::types types;

// 3. we can check everything at compile time
BOOST_STATIC_ASSERT((!boost::mpl::empty<types>::value));

// 4. we can also check other stuff
BOOST_STATIC_ASSERT((boost::is_same<non_defined, \
                     boost::mpl::at_c<types, 4>::type>::value));

// 5. and the last type is still std::string
BOOST_STATIC_ASSERT((boost::is_same<
                     boost::mpl::back<types>::type,
                     std::string>::value));

// 6. remove constant and volatile qualifiers
typedef boost::mpl::transform<types,
        boost::remove_cv<boost::mpl::_1>>::type noncv_type;

// 7. remove duplicate types
typedef boost::mpl::unique<
  noncv_types,
  boost::is_same<boost::mpl::_1, boost::mpl::_2>
  >::type unique_types;

// 8. check if the vector contains only 5 types
BOOST_STATIC_ASSERT((boost::mpl::size<unique_types>::value == 5));

// 9. compute sizes
struct non_defined {};

typedef boost::mpl::transform<
  unique_types,
  boost::mpl::sizeof_<boost::mpl::_1>
  >::type sizes_types;

// 10. get the maximum size
typedef boost::mpl::max_element<sizes_types>::type max_size_type;
BOOST_STATIC_ASSERT(max_size_type::type::value == sizeof(declared));


