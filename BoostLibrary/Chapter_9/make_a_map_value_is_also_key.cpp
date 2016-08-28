// 1. following includes
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

// 2. make vocabulary structure
typedef boost::bimap<std::string,
                     boost::bimaps::multiset_of<std::size_t>
                     > name_id_type;

name_id_type name_id;

// 3. can be filled using the following syntax
// insert keys<->values
name_id.insert(name_id_type::value_type("John Snow", 1));
name_id.insert(name_id_type::value_type("Vasya Pupkin", 2));
name_id.insert(name_id_type::value_type("Antony Polukhin", 3));
name_id.insert(name_id_type::value_type("Antony Polukhin", 3));

// 4. work with the left part of bimap just like with a map
std::cout << "Left: \n";
typedef name_id_type::left_const_iterator left_const_iterator;
for (left_const_iterator it = name_id.left.begin(), iend = name_id.left.end();
     it != iend; ++it) {
  std::cout << it->first << "<==>" << it->second << "\n";
 }

// 5. work with the right part of bimap just like with a map
std::cout << "\nRight: \n";
typedef name_id_type::right_const_iterator right_const_iterator;
for (right_const_iterator it = name_id.right.begin(), iend = name_id.right.end();
     it != iend; ++it) {
  std::cout << it->first << "<==>" << it->second << "\n";
 }

// 6. Ensure that there is such a person in the vocabulary

assert(name_id.find(name_id_type::value_type("Anton Polukhin", 3))
       != name_id.end());
