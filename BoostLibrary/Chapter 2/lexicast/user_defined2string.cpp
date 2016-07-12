#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <iosfwd>
#include <stdexcept>
#include <iostream>

class negative_number {
  unsigned short number_;
public:
  negative_number() {};
  explicit negative_number(unsigned short number) : number_(number) {};
  unsigned short value_without_sign() const { return number_; };
};

std::ostream &operator<<(std::ostream &os, const negative_number &num) {
  os << '-' << num.value_without_sign();
  return os;
}

std::istream &operator>>(std::istream &is, negative_number &num) {
  char ch;
  is >>ch;
  if (ch != '-') {
    throw std::logic_error("negative_number class designed to store \
                            only negative value");
  }
  unsigned short s;
  is >> s;
  num = negative_number(s);
  return is;
}

// convert to wide character strings, overloads are required
template<class CharT>
std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &os,
                                      const negative_number &num) {
  os << static_cast<CharT>('-') << num.value_without_sign();
  return os;
}

template<class CharT>
std::basic_istream<CharT> &operator>>(std::basic_istream<CharT> &is,
                                      negative_number &num) {
  CharT ch;
  is >> ch;
  if (ch != static_cast<CharT>('-')) {
    throw std::logic_error("Only handle negative number!");
  }
  unsigned short s;
  is >> s;
  num = negative_number(s);
  return is;
}

int main() {
  negative_number n(boost::lexical_cast<negative_number>("-100"));
  assert(n.value_without_sign() == 100);
  int i = boost::lexical_cast<int>(n);
  assert(i == -100);

  typedef boost::array<char, 10> arr_t;
  arr_t arr = boost::lexical_cast<arr_t>(n);
  assert(arr[0] == '-');
  assert(arr[1] == '1');
  assert(arr[2] == '0');

  // wide character strings
  negative_number nn(boost::lexical_cast<negative_number>(L"-1"));
  assert(nn.value_without_sign() == 1);
  typedef boost::array<wchar_t, 10> warr_t;
  warr_t warr = boost::lexical_cast<warr_t>(nn);
  assert(warr[0] == L'-');
  assert(warr[1] == L'1');
  assert(warr[2] == L'\0');
  return 0;
}
