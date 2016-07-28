#include <string>
std::string str1 = "Thanks for reading me!";
std::string str2 = "THANKS for readING ME!";
// case insensitive comparison

//1. most trivial one
#include <boost/algorithm/string/predicate.hpp>
boost::iequals(str1, str2);

//2. use boost predicate and STL method
#include <boost/algorithm/string/compare.hpp>
#include <algorithm>
str1.size() == str2.size() && std::equal(str1.begin(),
                                         str1.end(),
                                         str2.begin(),
                                         boost::is_iequal());

//3. make a lowercase copy of both the strings
#include <boost/algorithm/string/case_conv.hpp>
std::string str1_low = boost::to_lower_copy(str1);
std::string str2_low = boost::to_lower_copy(str2);
assert(str1_low == str2_low);

//4. make a uppercase copy
#include <boost/algorithm/string/case_conv.hpp>
std::string str1_up = boost::to_upper_copy(str1);
std::string str2_up = boost::to_upper_copy(str2);
assert(str1_up == str2_up);

//5. convert the original strings to lower case
#include <boost/algorithm/string/case_conv.hpp>
boost::to_lower(str1);
boost::to_lower(str2);
assert(str1 == str2);
