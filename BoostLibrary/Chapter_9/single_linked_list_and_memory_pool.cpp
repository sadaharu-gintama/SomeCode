// 1. Following headers
#include <boost/pool/pool_alloc.hpp>
#include <boost/container/slist.hpp>

// 2. now we need to describe the type of our list
typedef boost::fast_pool_allocator<int> allocator_t;
typedef boost::container::slist<int, allocator_t> slist_t;

// 3. now work with our single-linked list
template <class ListT>
void test_lists() {
  typedef ListT list_t;

  list_t list(100000, 0);
  for (int i = 0; i != 1000; ++i) {
    list.insert(list.begin(), 1);
  }

  typedef typename list_t::iterator iterator;
  iterator it = std::find(list.begin(), list.end(), 777);
  assert(it != list.end());

  for (int i = 0; i < 100; ++i) {
    list.pop_front();
  }

  assert(it != list.end());
  assert(*it == 777);

  for (int i = 0; i != 10; ++i) {
    list.insert(list.begin(), i);
  }

  assert(it != list.end());
  assert(*it == 777);

  list_specific(list, it);
}

// 4. feature specific for each type of list are moved to list_specific
void list_specific(slist_t &list, slist_t::iterator it) {
  typedef slist_t::iterator iterator;

  assert( *(++iterator(it)) == 776);
  assert(*it == 777);
  list.erase_after(it);
  assert(*it == 777);
  assert( *(++iterator(it)) == 775);

  boost::singleton_pool<
    boost:pool_allocator_tag, sizeof(int)>::release_memory();
}

#include <list>

typedef std::list<int> stdlist_t;

void list_specific(stdlist_t &list, stdlist_t::iterator it) {
  typedef stdlist_t::iterator iterator;
  ++it;
  assert(*it == 776);
  it = list.erase(it);
  assert(*it == 775);
}
