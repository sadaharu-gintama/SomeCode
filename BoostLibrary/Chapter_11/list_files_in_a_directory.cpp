// 1. include following headers
#include <boost/filesystem/operations.hpp>
#include <iostream>

// 2. now we need to specify a directory
int main() {
  boost::filesystem::directory_iterator begin("./");

  // 3. after specifying the directory, loop through its content
  boost::filesystem::directory_iterator end;
  for (; begin != end; ++begin) {
    // 4. get the file info
    boost::filesystem::file_status fs = boost::filesystem::status(*begin);
    // 5. now output the file info
    switch (fs.type()) {
    case boost::filesystem::regular_file:
      std::cout << "FILE       ";
      break;
    case boost::filesystem::symlink_file:
      std::cout << "SYMLINK    ";
      break;
    case boost::filesystem::directory_file:
      std::cout << "DIRECTORY  ";
      break;
    default:
      std::cout << "OTHER      ";
      break;
    }

    if (fs.permissions() & boost::filesystem::owner_write) {
      std::cout << "W  ";
    }
    else {
      std::cout << "   ";
    }

    //6. the final step will be output the file name
    std::cout << *begin << std::endl;
  }
}
