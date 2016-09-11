// 1. mark the move_nothrow assignment operator and move_nothrow constructor with the BOOST_NOEXCEPT macro:
#include <boost/config.hpp>
class move_nothrow {
  // some class member
  // ... ...
public:
  move_nothrow() BOOST_NOEXCEPT {};
  move_nothrow(move_nothrow &&) BOOST_NOEXCEPT // : members initialization
  {};
  move_nothrow &operator=(move_nothrow &&) BOOST_NOEXCEPT
  {
    // implementation
    return *this;
  }

  move_nothrow(const move_nothrow&);
  move_nothrow &operator=(const move_nothrow&);
};

// 2. now we may use the class with std::Vector in C++
std::vector<move_nothrow> v(10);
v.push_back(mow_nothrow());
