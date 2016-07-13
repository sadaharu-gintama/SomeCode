#include <set>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <cassert>

//##############################################################################
// Standard C++ without boost library
// store pointer in container and take care of their desctructions
template <class T>
struct ptr_cmp : public std::binary_function<T, T, bool> {
  template <class T1>
  bool operator() (const T1 &v1, const T1 &v2) const {
    return operator()(*v1, *v2);
  }
  bool operator() (const T &v1, const T &v2) const {
    return std::less<T>(v1, v2);
  }
};

void example1() {
  std::set<int *, ptr_cmp<int> > s;
  s.insert(new int(1));
  s.insert(new int(2));

  assert(**s.begin() == 0);

  //deallocate resources
  // if any exception happens, this code will lead to memory leakage
  std::for_each(s.begin(), s.end(), boost::bind(::operator delete, _1));
}

//##############################################################################
// store smart pointers in containers
// smart pointer is good, but still need to write a comparator functional object
void example2() {
  typedef std::unique_ptr<int> int_uptr_t;
  std::set<int_uptr_t, ptr_cmp<int> > s;
  s.insert(int_uptr_t(new int(1)));
  s.insert(int_uptr_t(new int(0)));

  assert(**s.begin() == 0);
  // resources will deallocate automatically by unique_ptr<>
}

//##############################################################################
// boost smart pointer in the container
#include <boost/shared_ptr.hpp>
void example3() {
  typedef boost::shared_ptr<int> int_sptr_t;
  std::set<int_sptr_t, ptr_cmp<int> > s;
  s.insert(int_sptr_t(new int(1)));
  s.insert(int_sptr_t(new_int(0)));
  assert(**s.begin() == 0);
  // resources will deallocate automatically by shared_ptr<>
}

//##############################################################################
// the recommended solution
#include <boost/ptr_container/ptr_set.hpp>
void correct_imp1() {
  boost::ptr_set<int> s;
  s.insert(new int(1));
  s.insert(new int(0));
  assert(**s.begin() == 0);
  // resources will deallocate automatically by container itself
}

