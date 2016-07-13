#include <boost/scope_exit.hpp>
#include <cstdlib>
#include <cstdio>
#include <cassert>

int main() {
  std::FILE *f = std::fopen("example.txt", "w");
  assert(f);
  BOOST_SCOPE_EXIT(f) {
    // code inside will be executed whatever happened in scope.
    // if pass by reference, take a & symbol
    // if multiple values, separate them using a comma
    std::fclose(f);
  } BOOST_SCOPE_EXIT_END
      // some code that may throw or return
}

// special symbol this_
class theres_more_example {
public:
  void close(std::FILE *);
  void theres_more_example_func() {
    std::FILE *f = 0;
    BOOST_SCOPE_EXIT(f, this_) {
      this_->close(f);
    } BOOST_SCOPE_EXIT_END
  }
};
