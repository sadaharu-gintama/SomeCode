#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

class foo_class {};
//
void process_sp1(const boost::shared_ptr<foo_class> &p);
void process_sp2(const boost::shared_ptr<foo_class> &p);
void process_sp3(const boost::shared_ptr<foo_class> &p);

void foo2() {
  typedef boost::shared_ptr<foo_class> ptr_t;
  ptr_t p;
  while (p = ptr_t(get_data())) {
    boost::thread(boost::bind(&process_sp1, p)).detach();
    boost::thread(boost::bind(&process_sp2, p)).detach();
    boost::thread(boost::bind(&process_sp3, p)).detach();
    // no need to deallocate the pointer p as it's a shared pointer
  }
}

//##############################################################################
// Another example
#include <string>
#include <boost/smart_ptr/make_shared.hpp>

void process_str1(boost::shared_ptr<std::string> p);
void process_str2(const boost::shared_ptr<std::string> &p);

void foo3() {
  boost::shared_ptr<std::string> ps =
    boost::make_shared<std::string>(
      "Guess why make_shared is fater than shared_ptr");
  boost::thread(boost::bind(&process_str1, ps)).detach();
  boost::thread(boost::bind(&process_str2, ps)).detach();
  // no need to deallocate the pointer ps
}
