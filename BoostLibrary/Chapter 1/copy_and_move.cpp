#include <boost/swap.hpp>
#include <boost/move/move.hpp>
#include <string>

namespace other {
  class characteristics{};
}

struct person_info {
private:
  BOOST_COPYABLE_AND_MOVABLE(person_info);
  // Fields declared here
public:
  bool is_male_;
  std::string name_;
  std::string second_name_;
  other::characteristics characteristics_;

  person_info() {};
  person_info(const person_info &p) : is_male_(p.is_male_),
                                      name_(p.name_),
                                      second_name_(p.second_name_),
                                      characteristics_(p.characteristics_) {};
  person_info(BOOST_RV_REF(person_info) person) { swap(person); };

  person_info& operator=(BOOST_COPY_ASSIGN_REF(person_info) person) {
    if (this != &person) {
      person_info tmp(person);
      swap(tmp);
    }
    return *this;
  };

  person_info& operator=(BOOST_RV_REF(person_info) person) {
    if (this != &person) {
      swap(person);
      person_info tmp;
      tmp.swap(person);
    }
    return *this;
  }

  void swap(person_info &rhs) {
    std::swap(is_male_, rhs.is_male_);
    name_.swap(rhs.name_);
    second_name_.swap(rhs.second_name_);
    // move assignment for user-defined class
    // because std::swap cannot be used here
    boost::swap(characteristics_, rhs.characteristics_);
  };
};

int main() {
  person_info vasya;
  vasya.name_ = "Vasya";
  vasya.second_name_ = "Snow";
  vasya.is_male_ = true;

  person_info new_vasya(boost::move(vasya));
  assert(new_vasya.second_name_ == "Snow");
  assert(vasya.second_name_.empty());

  vasya = boost::move(new_vasya);
  assert(vasya.name_ == "Vasya");
  assert(new_vasya.second_name_.empty());
}
