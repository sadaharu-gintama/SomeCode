#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <locale>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <deque>
// convert container of strings to vector of long int values
template <class ContainerT>
std::vector<long int> container2longs(const ContainerT &container) {
  typedef typename ContainerT::value_type value_type;
  std::vector<long int> ret;
  typedef long int (*func_t)(const value_type&);
  func_t f = &boost::lexical_cast<long int, value_type>;
  std::transform(container.begin(), container.end(),
                 std::back_inserter(ret), f);
  return ret;
};

int main() {
  // string to number
  int i = boost::lexical_cast<int>("100");
  std::cout << i << std::endl;
  // can also be used for non-zero terminated string
  char chars[] = {'1', '2', '3'};
  i = boost::lexical_cast<int>(chars, 3);
  std::cout << i << std::endl;
  // will check bounds and syntax of input :)
  try {
    short s = boost::lexical_cast<short>("9999999999999999999999999999999999");
    assert(false);
  }
  catch (const boost::bad_lexical_cast &) {};

  try {
    int s = boost::lexical_cast<int>("this is not a number");
    assert(false);
  }
  catch (const boost::bad_lexical_cast &) {};

  // convert localized numbers
  std::locale::global(std::locale("ru_RU.UTF8"));
  float f = boost::lexical_cast<float>("1,0");
  assert(f < 1.01 && f > 0.99);

  // template converts a container of string to vector of long int values
  std::set<std::string> str_set;
  str_set.insert("1");
  assert(container2longs(str_set).front() == 1);
  std::deque<const char*> char_deque;
  char_deque.push_front("1");
  char_deque.push_back("2");
  assert(container2longs(char_deque).front() == 1);

  typedef boost::array<unsigned char, 2> element_t;
  boost::array<element_t, 2> arrays = {{ {{'1', '0'}}, {{'2', '9'}} }};
  assert(container2longs(arrays).front() == 10);
  return 0;
}
