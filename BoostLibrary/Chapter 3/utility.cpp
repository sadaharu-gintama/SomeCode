#include <boost/noncopyable.hpp>
#include <sstream>

class tasks_processor : boost::noncopyable {
  std::ostream & log_;
protected:
  virtual void do_process() = 0;
public:
  explicit tasks_processor(std::ostream &log) : log_(log) {};
  void process() { log_ << "start data processing"; do_process(); };
};

// a derived class that will not compile
class fake_tasks_processor : public tasks_processor {
  std::ostringstream logger_;
  virtual void do_process() {
    logger_ << "Fake processor processed.";
  }
public:
  fake_tasks_processor() : tasks_processor(logger_), //logger doen't exist now?
                           logger_() {};
};

//##############################################################################
// boost::utility base_from_member template
#include <boost/utility/base_from_member.hpp>
class fake_tasks_processor_fixed
  : boost::base_from_member<std::ostringstream>
  , public tasks_processor {
  typedef boost::base_from_member<std::ostringstream> logger_t;
public:
  fake_tasks_processor_fixed()
    : logger_t(),
      tasks_processor(logger_t::member) {};
};

//##############################################################################
// when there are multiple base_from_member classes of same type:
class fake_tasks_processor2
  : boost::base_from_member<std::ostringstream, 0>
  , boost::base_from_member<std::ostringstream, 1>
  , public tasks_processor {
  typedef boost::base_from_member<std::ostringstream, 0> logger0_t;
  typedef boost::base_from_member<std::ostringstream, 1> logger1_t;

  virtual void do_process() {
    logger0_t::member << "0: Fake processor2 processed!";
    logger1_t::member << "1: Fake processor2 processed!";
  };
public:
  fake_tasks_processor2()
    : logger0_t()
    , logger1_t()
    , tasks_processor(logger0_t::member)
  {};
};
