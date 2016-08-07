#include <string>
#include <iostream>
// erase require the header below
#include <boost/algorithm/string/erase.hpp>
// replace require the header below
#include <boost/algorithm/string/replace.hpp>
namespace ba = boost::algorithm;

const std::string str = "Hello, hello, dear reader!";

int main() {
  // erase
  std::cout << "erase_all_copy: " << ba::erase_all_copy(str, ",") << std::endl;
  std::cout << "erase_first_copy: " << ba::erase_first_copy(str, ",") << std::endl;
  std::cout << "erase_last_copy: " << ba::erase_last_copy(str, ",") << std::endl;
  std::cout << "ierase_all_copy: " << ba::ierase_all_copy(str, "hello") << std::endl;
  std::cout << "ierase_nth_copy: " << ba::ierase_nth_copy(str, ",", 1) << std::endl;

  // replace
  std::cout << "replace_all_copy: " << ba::replace_all_copy(str, ",", "!") << std::endl;
  std::cout << "replace_first_copy: " << ba::replace_first_copy(str, ",", "!") << std::endl;
  std::cout << "replace_head_copy: " << ba::replace_head_copy(str, 6, "Whaaaaaa!") << std::endl;

  return 0;
}
