#include <string>
#include <algorithm>
#include <iostream>
#include <boost/utility/string_ref.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>


// standard C++ way of process
std::string between_str(const std::string &input, char starts, char ends) {
  std::string::const_iterator pos_beg = std::find(input.begin(), input.end(), starts);
  if (pos_beg == input.end()) {
    return std::string(); // An empty copy
  }

  ++pos_beg;
  std::string::const_iterator pos_end = std::find(input.begin(), input.end(), ends);
  return std::string(pos_beg, pos_end);
}

// boost library
// by doing so we avoid the overhead of creating dynamic memory and allocation
boost::string_ref between(const boost::string_ref &input, char starts, char ends) {
  boost::string_ref::const_iterator pos_beg = std::find(input.cbegin(), input.cend(), starts);
  if (pos_beg == input.cend()) {
    return boost::string_ref(); // An empty copy
  }

  ++pos_beg;
  boost::string_ref::const_iterator pos_end = std::find(input.cbegin(), input.cend(), ends);
  // changes here
  if (pos_end == input.cend()) {
    return boost::string_ref(pos_beg, input.end() - pos_beg);
  }
  return boost::string_ref(pos_beg, pos_end - pos_beg);
}

void string_ref_algorithm_examples() {
  boost::string_ref r("O_O");
  // finding symbol
  std::find(r.begin(), r.end(), '_');
  // will print 'o_o'
  boost::to_lower_copy(std::ostream_iterator<char>(std::cout), r);
  std::cout << "\n";
  // will print O_O
  std::cout << r << "\n";
  // will print "^_^"
  boost::replace_all_copy(std::ostream_iterator<char>(std::cout), r, "O", "^");
}


