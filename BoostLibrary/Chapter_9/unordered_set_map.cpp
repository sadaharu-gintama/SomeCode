// 1. include headers
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <string>
#include <iostream>
// 2. use unordered
void example() {
  boost::unordered_set<std::string> strings;
  strings.insert("This");
  strings.insert("is");
  strings.insert("an");
  strings.insert("example");

  assert(strings.find("is") != strings.end());
}

template<class T>
void output_example() {
  T strings;
  strings.insert("CZ");
  strings.insert("CD");
  strings.insert("A");
  strings.insert("B");

  std::copy(strings.begin(),
            strings.end(),
            std::ostream_iterator<std::string>(std::cout, " "));
}

template<class T>
std::size_t test_default() {
  // constants
  const std::size_t ii_max = 20000000;
  const std::string s("Test string");

  T map;

  for (std::size_t ii = 0; ii != ii_max; ++ii) {
    map[s + boost::lexical_cast<std::string>(ii)] = ii;
  }

  for (std::size_t ii = 0; ii != ii_max; ++ii) {
    map[s + boost::lexical_cast<std::string>(ii)] = ii;
  }
  return map.size();
};
