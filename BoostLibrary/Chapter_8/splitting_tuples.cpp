// 1. to implement that fix, we need the following headers
#include <boost/fusion/include/remove_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

// 2. we are ready to make a function that returns non-arithmetic types
template <class Sequence>
typename boost::fusion::result_of::remove_if<
  const Sequence,
  boost::is_arithmetic<boost::mpl::_1>
  >::type get_nonarithmetics(const Sequence &seq) {
  return boost::fusion::remove_if<
    boost::is_arithmetic<boost::mpl::_1>
    >(seq);
}

// 3. and a function that returns arithmetic types
template <class Sequence>
typename boost::fusion::result_of::remove_if<
  const Sequence,
  boost::mpl::not_<boost::is_arithmetic<boost::mpl::_1> >
  >::type get_arithmetics(const Sequence &seq) {
  return boost::fusion::remove_if<
    boost::mpl::not_<boost::is_arithmetic<boost::mpl::_1> >
    >(seq);
}

// that's it. Now we are capable of doing the following tasks
#include <boost/fusion/include/vector.hpp>
#include <cassert>
#include <boost/fusion/include/at_c.hpp>
#include <boost/blank.hpp>

int main() {
  typedef boost::fusion::vector<
    int, boost::blank, boost::blank, float
    > tup1_t;

  tup1_t tup1(8, boost::blank(), boost::blank(), 0.0);

  boost::fusion::vector<boost::blank, boost::blank> res_na = get_nonarithmetics(tup1);
  boost::fusion::vector<int, float> res_a = get_arithmetics(tup1);

  assert(boost::fusion::at_c<0>(res_a) == 8);
  return 0;
}
