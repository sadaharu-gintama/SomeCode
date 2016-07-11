#include <boost/noncopyable.hpp>
// by inheriting from boost::noncopyable,
// we cannot either assign nor copy the class
// similarly we can make copy/assignment operator private
// or we can declare but not define copy/assignment operator
// but this is more neat and clear
class descriptor_owner: private boost::noncopyable {
  void *descriptor_;
public:
  explicit descriptor_owner(const char* params);
  ~descriptor_owner() {
    delete [] descriptor_;
  }
};

