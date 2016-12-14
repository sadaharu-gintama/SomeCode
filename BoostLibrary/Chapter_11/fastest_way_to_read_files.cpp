// 1. include boost library
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <algorithm>
// 2. open a file
const boost::interprocess::mode_t mode = boost::interprocess::read_only;
boost::interprocess::file_mapping fm(filename, mode);

// 3. main part is mapping all of the files to memory
boost::interprocess::mapped_region region(fm, mode, 0, 0);

// 4. getting a pointer to the data in the files
const char *begin = reinterpret_cast<const char*>(region.get_address());

// now we can work with a file just as with normal memory
const char *pos = std::find(begin, begin + region.get_size(), '\1');
