#include <map>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <string>

struct user_info {
  std::string address;
  unsigned short age;
  // Other parameters
  // ...
};

class users_online {
  typedef boost::mutex mutex_t;
  mutable mutex_t users_mutex_;
  std::map<std::string, user_info> users_;
public:
  bool is_online(const std::string &username) const {
    boost::lock_guard<mutex_t> lock(users_mutex_);
    return users_.find(username) != users_.end();
  };

  unsigned short get_age(const std::string &username) const {
    boost::lock_guard<mutex_t> lock(users_mutex_);
    return users_.at(username).age;
  }

  void set_online(const std::string &username, const user_info &data) {
    boost::lock_guard<mutex_t> lock(users_mutex_);
    users_.insert(std::make_pair(username, data));
  }

  // Other method
  // ......
};

// Any operatoion will acquire a unique lock on the mutex_ variable, so even
// getting resources will result in waiting on a locked mutex
// replace unique_lock by shared_locked
#include <boost/thread/shared_mutex.hpp>
class users_online {
  typedef boost::shared_mutex mutex_t;
  mutable mutex_t users_mutex_;
  std::map<std::string, user_info> users_;
public:
  bool is_online(const std::string &username) const {
    boost::shared_lock<mutex_t> lock(users_mutex_);
    return users_.find(username) != users_.end();
  };

  unsigned short get_age(const std::string &username) const {
    boost::shared_lock<mutex_t> lock(users_mutex_);
    return users_.at(username).age;
  }

  // here we still use lock_guard. so that only one thread can modify
  // the map
  void set_online(const std::string &username, const user_info &data) {
    boost::lock_guard<mutex_t> lock(users_mutex_);
    users_.insert(std::make_pair(username, data));
  }

  // Other method
  // ......
};
