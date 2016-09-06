#include <string>
#include <iostream>

struct person {
  std::size_t id_;
  std::string name_;
  unsigned int height_;
  unsigned int weight_;
  person(std::size_t id, const std::string &name,
         unsigned int height, unsigned int weight) :
    id_(id), name_(name), height_(height), weight_(weight) {};
};

inline bool operator <(const person &p1, const person &p2) {
  return p1.name_ < p2.name_;
}

// 1. need a lot of includes
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

// 2. construct the multi-index type
typedef boost::multi_index::multi_index_container<
  person, boost::multi_index::indexed_by<
// names are unique
            boost::multi_index::ordered_unique<
              boost::multi_index::identity<person>
              >,
// IDs are not unique, but we do not need then ordered
            boost::multi_index::hashed_non_unique<
              boost::multi_index::member<
                person, std::size_t, &person::id_>
              >,
// Height may not be unique, but must be ordered
            boost::multi_index::ordered_non_unique<
              boost::multi_index::member<
                person, unsigned int, &person::height_>
              >,
// Weight may not be unique, but muse be ordered
            boost::multi_index::ordered_non_unique<
              boost::multi_index::member<
                person, unsigned int, &person::weight_>
              >
            > // closing of indexed_by
  > indexes_t;

// 3. now we may insert values into our multi-index
indexes_t persons;

// insert value
persons.insert(person(1, 'John Snow', 195, 85));
persons.insert(person(2, 'Vasya Pupkin', 165, 65));
persons.insert(person(3, 'Antony Polukhin', 183, 70));
persons.insert(person(3, 'Antony Polukhin', 182, 70));

// 4. Construct a function for printing the index content
template <std::size_t IndexNo, class Indexes>
void print(const Indexes &persons) {
  std::cout << IndexNo << ":\n";
  typedef typename Indexes::template nth_index<
    IndexNo>::type::const_iterator const_iterator_t;

  for (const_iterator_t it = persons.template get<IndexNo>().begin(),
         iend = persons.template get<IndexNo>().end();
       it != iend; ++it) {
    const person &v = *it;
    std::cout << v.name_ << ","
              << v.id_ << ","
              << v.height_ << ","
              << v.weight_ << '\n';
  }
  std::cout << '\n';
};

// 5. print all the indexes as follows:
print<0>(persons);
print<1>(persons);
print<2>(persons);
print<3>(persons);

// 6. some code from the previous recipe can also be used
assert(personas.get<1>().find(2)->name_ == "Vasya Pupkin");
assert(persons.find(person(77, "Anton Polukhin", 0, 0)) != persons.end());
