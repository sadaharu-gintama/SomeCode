// 1. we will need the following headers to write a higher-order metafunction
#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

// 2. evaluate our functions
template <class Param1, class Param2, class Func1, class Func2>
struct coalesce {
  typedef typename boost::mpl::apply<Func1, Param1>::type type1;
  typedef typename boost::mpl::apply<Func2, Param2>::type type2;
  // 3. now we need to choose the correct result type
  typedef typename boost::mpl::if_<
    boost::is_same<boost::mpl::false_, type1>,
    type2,
    type1
    >::type type;
};

// now we can use it
#include <boost/static_assert.hpp>
#include <boost/mpl/not.hpp>
using boost::mpl::_1;
using boost::mpl::_2;

typedef coalesce<
  boost::mpl::true_,
  boost::mpl::true_,
  boost::mpl::not_<_1>,
  boost::mpl::not_<_1>
  >::type res1_t;
BOOST_STATIC_ASSERT((!res1_t::value));

typedef coalesce<
  boost::mpl::true_,
  boost::mpl::false_,
  boost::mpl::not_<_1>,
  boost::mpl::not_<_1>
  >::type res2_t;
BOOST_STATIC_ASSERT((res2_t::value));
