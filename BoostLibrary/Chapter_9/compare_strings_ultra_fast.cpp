#include <string>
template <class T>
std::size_t test_default() {
  //constants
  const std::size_t ii_max = 20000000;
  const std::string s("Long Long Long String that"
                      "will be used in tests to compare"
                      "speed of equality comparison.");
  // make some data that will be used in comparison
  const T data[] = {T(s),
                    T(s + s),
                    T(s + "Woooooo."),
                    T(std::string(""))};

  const std::size_t data_dimensions = sizeof(data) / sizeof(data[0]);

  std::size_t matches = 0u;

  for (std::size_t ii = 0; ii != ii_max; ++ii) {
    for (std::size_t i = 0; i < data_dimensions; ++i) {
      for (std::size_t j = 0; j != data_dimensions; ++j) {
        if (data[i] == data[j]) {
          ++matches;
        }
      }
    }
  }

  return matches;
};

// 1. Following headers are needed
#include <boost/functional/hash.hpp>

// 2. Create our fast comparison class
struct string_hash_fast {
  typedef std::size_t comp_type;
  const comp_type comparison_;
  const std::string str_;

  explicit string_hash_fast(const std::string &s)
    : comparison_(boost::hash<std::string>()(s)), str_(s) {};
};

// 3. Add equality comparison operators
inline bool operator== (const string_hash_fast &s1,
                        const string_hash_fast &s2) {
  return s1.comparison_ == s2.comparison_ &&
    s1.str_ == s2.str_;
}

inline bool operator!=(const string_hash_fast &s1,
                       const string_hash_fast &s2) {
  return !(s1 == s2);
}


#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    assert(
           test_default<string_hash_fast>() ==
           test_default<std::string>());
    return 0;
  }

  switch (argv[1][0]) {
  case 'h':
    std::cout << "HASH Matched: "
              << test_default<string_hash_fast>();
    break;
  case 's':
    std::cout << "STD Matched: "
              << test_default<std::string>();
    break;
  default:
    assert(false);
    return -2;
  }

  return 0;
}
