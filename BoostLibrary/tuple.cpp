#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>

int main() {
  boost::tuple<int, std::string> almost_a_pair(10, "hello");
  boost::tuple<int, float, double, int> quad(10, 10.0, 10.0, 1);
  int i = boost::get<0>(almost_a_pair);
  const std::string &str = boost::get<1>(almost_a_pair);
  double d = boost::get<2>(quad);
  std::cout << i << "," << str << "," << d << std::endl;

  std::set<boost::tuple<int, double, int> > s;
  s.insert(boost::make_tuple(1, 1., 2));
  s.insert(boost::make_tuple(2, 10., 2));
  s.insert(boost::make_tuple(3, 100., 3));

  auto t = boost::make_tuple(0, -1., 2);
  assert(2 == boost::get<2>(t));

  boost::tuple<int, float, double, int> quad(10, 1., 10., 1);
  float f;
  int i2;

  boost::tie(i, f, d, i2) = quad;

  assert(i == 10);
  assert(i2 == 1);
  return 0;
}
