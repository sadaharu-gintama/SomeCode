#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
  typedef boost::variant<int, const char *, std::string> my_var_t;
  std::vector<my_var_t> some_vector;
  some_vector.push_back(10);
  some_vector.push_back("Hello, there");
  some_vector.push_back(std::string("Fork"));
  std::string &s = boost::get<std::string>(some_vector.back());
  s += "That's great\n";
  std::cout << s << std::endl;
  return 0;
}
