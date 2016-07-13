#include <functional>
#include <boost/function.hpp>

// making a typedef for function pointer accepting int and returning nothing
typedef void (*func_t)(int);
// function that accepts pointer to function and calls accepted
// function for each integer that it has
// it cannot work with functional objects
void procees_integers(func_t f);
// Functional object
class int_processor : public std::unary_function<int, void> {
  const int min_;
  const int max_;
  bool &triggered_;
public:
  int_processor(int min, int max, bool &triggered) :
    min_(min), max_(max), triggered_(triggered) {};
  void operater()(int i) const {
    if (i < min_ || i > max_)
      triggered_ = true;
  }
};
//##############################################################################
// function object
typedef boost::function<void(int)> fobject_t;

// Now this function may accept functional objects
void process_integers(const fobject_t &f);

int main() {
  bool is_triggered = false;
  int_processor fo(0, 200, is_triggered);
  process_integers(fo);
  assert(is_triggered);
}

//##############################################################################
// function pointer
void my_ints_function(int i);

int main() {
  process_integers(&my_ints_function);
}

//##############################################################################
// lambda function takes no parameters and does nothing
process_integers([](int /*i*/) {});

// lambda that stores a reference
std::deque<int> ints;
process_integers([&ints](int i) {ints.push_back(i)});

// lambda function that modifies its content
std::size_t match_count = 0;
process_integers([ints, &match_count](int i) mutable {
    if (ints.front() == i)
      ++ match_count;
    ints.pop_front();
  });

