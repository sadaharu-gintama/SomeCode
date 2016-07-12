#include <boost/optional.hpp>
#include <iostream>
#include <stdlib.h>

class locked_device {
  explicit locked_device(const char*) {
    std::cout << "Device is locked" << std::endl;
  }
public:
  ~locked_device() {
    // release device lock
  }

  void use() {
    std::cout << "Success!" << std::endl;
  }

  static boost::optional<locked_device> try_lock_device() {
    if (rand() % 2) {
      return boost::none; // Failed to lock device
    }
    return locked_device("device name");
  }
};

int main() {
  srandom(5);
  for (unsigned int it = 0; it != 10; ++it) {
    boost::optional<locked_device> t = locked_device::try_lock_device();
    if (t) {
      t->use();
      return 0;
    }
    else {
      std::cout << "... try again later" << std::endl;
    }
  }
  std::cout << "Failure" << std::endl;
  return -1;
}
