// in C++11 we can use auto for the case below, but in C++03 cannot
#include <functional>
#include <boost/bind.hpp>
// C++03 cannot handle this
const ??? var = boost::bind(std::plus<int>(), _1, _1);

// boost typeof library can get the returning type of expression
#include <boost/typeof/typeof.hpp>
BOOST_AUTO(var, boost::bind(std::plus<int>(), _1, _1));

//C++11
typedef decltype(0.5 + 0.5f) type;
// C++03 BOOST library version
typedef BOOST_TYPEOF(0.5 + 0.5f) type;

// C++11 version's decltype(expr) deduces and returns the type of expr
template<class T1, class T2>
auto add(const T1 &t1, const T2 &t2)->decltype(t1 + t2) {
  return t1 + t2;
}

// using boost.typeof
template<class T1, class T2>
BOOST_TYPEOF_TPL(T1() + T2()) add(const T1 &t1, const T2 &t2) {
  return t1 + t2;
}

// freely use the results of BOOST_TYPEOF functions in templates and in any
// other compile time expressions
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
BOOST_STATIC_ASSERT((boost::is_same<BOOST_TYPE_OF(add(1, 1)), int>::value));

// but unfortunately, this magic does not always work without help
// for example, user-defined classes are not always detected
// following code may fail on some compilers:
namespace readers_project {
  template<class T1, class T2, class T3>
  struct readers_template_class {};
}

#include <boost/tuple/tuple.hpp>
typedef readers_project::readers_template_class<int, int, float>
  readers_template_class_1;

typedef BOOST_TYPEOF(boost::get<0>(
  boost::make_tuple(readers_template_class_1(), 1)))
  readers_template_class_deduced;

BOOST_STATIC_ASSERT((boost::is_same<
                     readers_template_class_1,
                     readers_template_class_deduced>::value));

// you may give boost::typeof a helping hand and register a template
BOOST_TYPEOF_REGISTER_TEMPLATE(readers_project::readers_template_class, 3);
