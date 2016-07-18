// Each task there can be executed in one of many threads and we don not know
// which one. Imagine that we want to send the result of an executed task using
// some connection.
#include <boost/noncopyable.hpp>
class connection: boost::noncopyable {
public:
  // opening a connection is a slow operation
  void open();

  void send_result(int result);

  // Other method
  // ......
};

// in header file
#include <boost/thread/tss.hpp>
connection & get_connection();

// in source file
boost::thread_specific_ptr<connection> connection_ptr;
connection & get_connection() {
  connection *p = connection_ptr.get();
  if (!p) {
    connection_ptr.reset(new connection);
    p = connection_ptr.get();
    p->open();
  }
  return *p;
}

// using a thread-specific resource was never so easy:
void task() {
  // some computations go there
  // ......

  //sending result
  get_connection().send_result(result);
}
