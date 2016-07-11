#include <boost/numeric/conversion/cast.hpp>
#include <iostream>
void some_function(unsigned short param) {};

int foo() {return 0;};
// boost way of converting
void correct_implementation() {
  some_function(boost::numeric_cast<unsigned short>(foo()));
}

void test_function() {
  for (unsigned int i = 0; i < 100; ++i) {
    try {
      correct_implementation();
    }
    catch (const boost::numeric::bad_numeric_cast &e) {
      std::cout << "#" << i << " " << e.what() << std::endl;
    }
  }
}

// we can even detect specific overflow type
void test_function1() {
  for (unsigned int i = 0; i < 100; ++i) {
    try {
      correct_implementation();
    }
    catch (const boost::numeric::positive_overflow &e) {
      std::cout << "#" << i << " POSITIVE OVERFLOW" << e.what() << std::endl;
    }
    catch (const boost::numeric::negative_overflow &e) {
      std::cout << "#" << i << " NEGATIVE OVERFLOW" << e.what() << std::endl;
    }
  }
}

//how to make my own overflow handler for boost::numeric::cast
template<class SourceT, class TargetT>
struct mythrow_overflow_handler {
  void operator() (boost::numeric::range_check_result r) {
    if (r != boost::numeric::cInRange) {
      throw std::logic_error("Not in range!");
    }
  }
};

template<class SourceT, class TargetT>
TargetT my_numeric_cast(const SourceT& in) {
  using namespace boost;
  typedef numeric::conversion_traits<TargetT, SourceT> conv_traits;
  typedef numeric::numeric_cast_traits<TargetT, SourceT> cast_traits;
  typedef boost::numeric::converter<TargetT, SourceT, conv_traits,
                                    mythrow_overflow_handler<SourceT, TargetT>
                                    > converter;
  return converter::convert(in);
};

try {
  mynumeric_cast<short>(100000);
}
catch (const std::logic_error &e) {
  std::cout << "It works! " << e.what() << std::endl;
}
