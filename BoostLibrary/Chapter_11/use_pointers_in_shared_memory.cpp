#include <boost/interprocess/offset_ptr.hpp>

struct correct_struct {
  boost::interprocess::offset_ptr<int> pointer_;
  // ...
  int value_holder_;
};


// now we are free to use it as a normal pointer
correct_struct &ref = *segment.construct<correct_struct>("structure")();
ref.pointer_ = &ref.value_holder_;
assert(ref.pointer_ == &ref.value_holder_);
assert(*ref.pointer_ == ref.value_holder_);

ref.value_holder_ = ethalon_value;
assert(*ref.pointer_ == ethalon_value);
