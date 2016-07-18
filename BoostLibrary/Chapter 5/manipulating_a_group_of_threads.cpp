// use boost::thread_group class
boost::thread_group threads;

for (unsigned int = 0; i < 10; ++i)
  threads.create_thread(&some_function);

// now we can call functions for all the threads inside boost::thread_group:
threads.join_all();
// we can also interrupt all of them by calling threads.interrupt_all()
