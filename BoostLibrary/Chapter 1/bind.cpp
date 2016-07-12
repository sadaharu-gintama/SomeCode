#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/ref.hpp>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

//##############################################################################
class device1 {
private:
  short temperature();
  short wetness();
  int illumination();
  int atmospheric_pressure();
  void wait_for_data();
public:
  template <class FuncT>
  void watch(const FuncT& f) {
    for (;;) {
      wait_for_data();
      f (temperature(), wetness(), illumination(), atmospheric_pressure());
    }
  }
};

class device2 {
private:
  short temperature();
  short wetness();
  int illumination();
  int atmospheric_pressure();
  void wait_for_data();
public:
  template <class FuncT>
  void watch(const FuncT& f) {
    for (;;) {
      wait_for_data();
      f (temperature(), wetness(), atmospheric_pressure(), illumination());
    }
  }
};
// Original function
void detect_storm(int wetness, int temperature, int atmospheric_pressure);

//boost::bind
// one detect_storm function for both classes
device1 dv1;
dv1.watch(boost::bind(&detect_storm, _2, _1, _4));

device2 dv2;
dv2.watch(boost::bind(&detect_storm, _1, _2, _3));

//##############################################################################
template<class FuncT>
void watch(const FuncT& f) {
  f(10, std::string("string"));
  f(10, "Char array");
  f(10, 10);
}

struct templated_foo {
  template<class T>
  void operator() (T, int) const {
    // no implementation here, just to show that
    // bound functions still can be used as templated
  }
};

void check_templated_bind() {
  watch(boost::bind<void>(templated_foo(), _2, _1));
}
//##############################################################################
// bind a value as function parameters
//count number greater than or equal to 5 in the following code
boost::array<int, 12> values = {{1,2,3,4,5,6,7,100,99,87,23,35}};
std::size_t count0 = std::count_if(values.begin(), values.end(),
                                   std::bind1st(std::less<int>(), 5));
std::size_t count1 = std::count_if(values.begin(), values.end(),
                                   boost::bind(std::less<int>(), 5, _1));

assert(count0 == count1);

// count empty strings
boost::array<std::string, 3> str_values = {{"We", "are", "the champions!"}};
count0 = std::count_if(str_values.begin(), str_values.end(),
                       std::mem_func_ref(&std::string::empty));
count1 = std::count_if(str_values.begin(), str_values.end(),
                       boost::bind(&std::string::empty, _1));
assert(count0 == count1);

// count strings that less than 5
std::size_t count = std::count_if(str_values.begin(), str_values.end(),
                                  boost::bind(std::less<std::size_t>(),
                                  boost::bind(&std::string::size, _1), 5));
// compare the strings
std::string s("Expensive copy constructor of std::string will \
              be called when binding");

count0 = std::count_if(str_values.begin(), str_values.end(),
                       std::bind2nd(std::less<std::string>(), s));

count1 = std::count_if(str_values.begin(), str_values.end(),
                       boost::bind<std::less<std::string>(), _1, s);

assert(count0 == count1);

//##############################################################################
// use cref to pass a parameter as a reference/const reference
std::string s("Expensive copy constructor of std::string now won't be called");
count0 = std::count_if(str_values.begin(), str_values.end(),
                       std::bind2nd(std::less<std::string>(), boost::cref(s)));

count1 = std::count_if(str_values.begin(), str_values.end(),
                       boost::bind<std::less<std::string>(), _1, boost::cref(s));

assert(count0 == count1);

//boost cref can be used to concatenate strings
void wierd_appender(std::string &to, const std::string &from) {
  to += from;
}

std::string result;
std::for_each(str_values.cbegin(), str_values.cend(),
              boost::bind(&wierd_appender, boost::ref(result), _1));
assert(result == "We are the Champions!");
