#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
int main() {
  // boost way of converting number to string
  std::string s = boost::lexical_cast<std::string>(1024);
  assert(s == "1024");

  // compare with traditional C++ conversion method
  std::stringstream ss;
  ss << 1024;
  s.clear();
  ss >> s;
  assert(s == "1024");

  // C traditional conversion method
  char buffer[100];
  std::sprintf(buffer, "%i", 1024);
  std::string s_c(buffer);
  assert(s_c == "1024");
  return 0;
}
