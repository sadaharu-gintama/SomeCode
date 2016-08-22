// 1. start with headers
// we will need the following in step 3
#include <boost/mpl/size.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
// we will need the following in step 4
#include <boost/mpl/if.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/add_const.hpp>
// we will need this in step 5
#include <boost/mpl/transform.hpp>
// should be used in test
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
// Assume we have the following structure
struct unsigne;
struct constant;
struct no_change;

// 2. put metaprogramming magic inside the structure
template <class Types, class Modifiers>
struct do_modifications {
// 3. it's a good idea to check that the passed vectors have the same size
  BOOST_STATIC_ASSERT((boost::is_same<
                       typename boost::mpl::size<Types>::type,
                       typename boost::mpl::size<Modifiers>::type>::value));
// 4. take care of modifying the metafunction
  typedef boost::mpl::if_<
    boost::is_same<boost::mpl::_2, unsigne>,
    boost::mpl::if_<
      boost::is_same<boost::mpl::_2, constant>,
      boost::add_const<boost::mpl::_1>,
      boost::mpl::_1
      >
    > binary_operator_t;
// 5. and the final step
  typedef typename boost::mpl::transform<
    Types,
    Modifiers,
    binary_operator_t
    >::type type;
};

// TEST to see if our metafunction works correctly
typedef boost::mpl::vector<unsigne, no_change, constant, unsigne> modifiers;
typedef boost::mpl::vector<int, char, short, long> types;
typedef do_modifications<types, modifiers>::type result_type;

BOOST_STATIC_ASSERT((boost::is_same<
                     boost::mpl::at_c<result_type, 0>::type,
                     unsigned int
                     >::value));

BOOST_STATIC_ASSERT((boost::is_same<
                     boost::mpl::at_c<result_type, 1>::type,
                     char
                     >::value));

BOOST_STATIC_ASSERT((boost::is_same<
                     boost::mpl::at_c<result_type, 2>::type,
                     const short
                     >::value));

BOOST_STATIC_ASSERT((boost::is_same<
                     boost::mpl::at_c<result_type, 3>::type,
                     unsigned long
                     >::value));


// step 3 can be rewritten as
BOOST_STATIC_ASSERT((
                     boost::mpl::size<Types>::type::value ==
                     boost::m[l::size<Modifiers>::type::value));
