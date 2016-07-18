// sometimes we need to kill a thread that ate too much resources or that is
// just executing too long
boost::thread parser_thread(&do_parse);
  // Some code goes here
  // ...
  if (stop_parsing) {
    // no more parsing required
    // TODO: stop parser
    parser_thread.interrupt();
  }

// if the thread was interrupted, the exception boost::thread_interrupted
// is thrown
// boost::thread_interrupted is not derived from std::exception!

// we may also add interruption points at any point. All we need is to call
// boost::this_thread::interruption_point()
void do_parse() {
  while (not_end_of_parsing) {
    boost::this_thread::interruption_point();
    // some parsing goes here
  }
}
// If interruptions are not required for a project, defining
// BOOST_THREAD_DONT_PROVIDE_INTERRUPTIONS give a small performance boost
// and totally disables thread interruptions
