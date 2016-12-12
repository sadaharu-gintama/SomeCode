// 1. include headers
#include <boost/filesystem/operations.hpp>
#include <cassert>
#include <fstream>
#include <iostream>

// 2. need a variable to store errors
int main() {
  boost::system::error_code error;
  // 3. create directories if required
  boost::filesystem::create_directories("dir/subdir", error);
  assert(!error);
  // 4. write data to file
  std::ofstream ofs("dir/subdir/file.txt");
  ofs << "Boost file system is fun" << std::endl;
  assert(ofs);
  ofs.close();
  // 5. we need to attempt to create symlink:
  boost::filesystem::create_directory_symlink("dir/subdir", "symlink", error);
  // 6. we need to check that the file is accessible through symlink
  if (!error) {
    std::cerr << "Symlink createt\n";
    assert(boost::filesystem::exists("symlink/file.txt"));
  }
  // 7. remove the created file if symlink creation failed
  else {
    std::cerr << "Failed to create a symlink\n";
    boost::filesystem::remove("dir/subdir/file.txt", error);
    assert(!error);
  }

  return 0;
}
