// 1. include header
#include <boost/container/flat_set.hpp>
// 2. construct the flat container
boost::container::flat_set<int> set;
// 3. reserving space for elements
set.reserve(4096);
// 4. fill the container
for (int i = 0; i != 4000; ++i) {
  set.insert(i);
 }
// 5. work with it just like with std::set
assert(set.lower_bound(600) - set.lower_bound(100) == 400);
set.erase(0);
set.erase(5000);
assert(std::lower_bound(set.cbegin(), set,cend(), 90000) == set.cend());
assert(set.lower_bound(100) + 400 == set.find(500));
