#include <boost/any.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
  std::vector<boost::any> some_vector;
  some_vector.push_back(12.0);
  const char *c_str = "Hello from the other side";
  some_vector.push_back(c_str);
  some_vector.push_back(std::string("Hello"));
  std::string &s = boost::any_cast<std::string &>(some_vector.back());
  s += "That is great\n";
  std::cout << s << std::endl;
  return 0;
}
