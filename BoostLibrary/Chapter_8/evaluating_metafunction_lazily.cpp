struct fallback;
template <
  class Func,
  class Param,
  class Cond,
  class Fallback = fallback>
struct apply_if;

// 1. Following headers
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

// 2. beginning of the function is simple
template <class Func, class Param, class Cond, class Fallback>
struct apply_if {
  typedef typename boost::mpl::apply<
    Cond, Param
    >::type condition_t;
// 3. but we need be CAREFUL here
  typedef boost::mpl::apply<Func, Param> applied_type;
// 4. Additional care must be taken
  typedef typename boost::mpl::eval_if_c<
    condition_t::value,
    applied_type,
    boost::mpl::identity<Fallback>
    >::type type;
};

// Now that's it, we are free to use it
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_same.hpp>

using boost::mpl::_1;
using boost::mpl::_2;

typedef apply_if<
  boost::make_unsigned<_1>,
  int,
  boost::is_integral<_1>
  >::type res1_t;

BOOST_STATIC_ASSERT((boost::is_same<res1_t, unsigned int>::value));

typedef apply_if<
  boost::make_unsigned<_1>,
  float,
  boost::is_integral<_1>
  >::type res2_t;

BOOST_STATIC_ASSERT((boost::is_same<res2_t, fallback>::value));
