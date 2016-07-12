#include <boost/cast.hpp>

// an already-designed object (not a good way)
struct object {
  virtual ~object() {};
};

struct banana : public object {
  void eat() const {};
  virtual ~banana() {};
};

struct pidgin : public object {
  void fly() const {};
  virtual ~pidgin() {};
};

object *try_produce_banana();

// our task is to make a function that eats bananas
// and throws exceptions if something instead of banana came

// 1. ugly
void try_eat_banana_imp1() {
  const object *obj = try_produce_banana();
  if (!obj) {
    throw std::bad_cast();
  }
  dynamic_cast<const banana &>(*obj).eat();
}

// 2. elegant way of using boost
void try_eat_banana_imp2() {
  const object *obj = try_produce_banana();
  boost::polymorphic_cast<const banana *>(obj)->eat();
}
