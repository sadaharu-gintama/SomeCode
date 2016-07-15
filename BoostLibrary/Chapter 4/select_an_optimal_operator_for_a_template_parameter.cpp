// something like this
template<class T>
void inc(T &value) {
  // call ++value
  // or call value++
  // or value += T(1)
  // or value = value + T(1)
};

// Let's start from making correct functional objects
namespace detail {
  struct pre_inc_functor {
    template<class T>
    void operator()(T &value) const {
      ++value;
    }
  };

  struct post_inc_functor {
    template<class T>
    void operator()(T &value) const {
      value++;
    }
  };

  struct plus_assignable_functor {
    template<class T>
    void operator()(T &value) const {
      value += T(1);
    }
  };

  struct plus_functor {
    template<class T>
    void operator()(T &value) const {
      value = value + T(1);
    }
  };
}; // namespace detail

// now we need a bunch of type traits
#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/has_plus_assign.hpp>
#include <boost/type_traits/has_plus.hpp>
#include <boost/type_traits/has_post_increment.hpp>
#include <boost/type_traits/has_pre_increment.hpp>

// deduce correct functor and use
template<class T>
void inc(T &value) {
  typedef detail::plus_functor step_0_t;

  typedef typename boost::conditional<
    boost::has_plus_assign<T>::value,
    detail::plus_assignable_functor,
    step_0_t>::type step_1_t;

  typedef typename boost::conditional<
    boost::has_post_increment<T>::value,
    detail::post_inc_functor,
    step_1_t>::type step_2_t;

  typedef typename boost::conditional<
    boost::has_pre_increment<T>::value,
    detail::pre_inc_functor,
    step_2_t>::type step_3_t;

  step_3_t()  // default constructing functor
    (value); // calling operator () of a functor
};

// Another version
// using boost::mpl
#include <boost/mpl/if.hpp>
template<class T>
void inc_mpl(T &value) {
  typedef detail::plus_functor step_0_t;

  typedef typename boost::mpl::if_<
    boost::has_plus_assign<T>,
    detail::plus_assignable_functor,
    step_0_t>::type step_1_t;

  typedef typename boost::mpl::if_<
    boost::has_post_increment<T>,
    detail::post_inc_functor,
    step_1_t>::type step_2_t;

  typedef typename boost::mpl::if_<
    boost::has_pre_increment<T>,
    detail::pre_inc_functor,
    step_2_t>::type step_3_t;

  step_3_t()(value);
};
