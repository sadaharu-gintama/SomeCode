// 1. we need to write a functor that converts any type to a string
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <string>

struct stringize_functor : boost::noncopyable {
private:
  std::string &result;
public:
  explicit stringize_functor(std::string &res) : result(res) {};
  template <class T> void operator() (const T &v) const {
    result += boost::lexical_cast<std::string>(v);
  };
};

// 2. and this is the tricky part of the code
#include <boost/fusion/include/for_each.hpp>
template <class Sequence>
std::string stringize(const Sequence &seq) {
  std::string result;
  boost::fusion::for_each(seq, stringize_functor(result));
  return result;
};

// All we can convert anything to a string now
struct cat {};

std::ostream &operator << (std::ostream &os, const cat &) {
  return os << "Meow!";
}


#include <iostream>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/container/vector.hpp>
int main() {
  boost::fusion::vector<cat, int, std::string> tup1(cat(), 0, "_0");
  boost::tuple<cat, int, std::string> tup2(cat(), 0, "_0");
  std::pair<cat, cat> cats;
  boost::array<cat, 10> many_cats;
  std::cout << stringize(tup1) << "\n"
            << stringize(tup2) << "\n"
            << stringize(cats) << "\n"
            << stringize(many_cats) << "\n";
  return 0;
}
