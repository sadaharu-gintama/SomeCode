// 1. include following header
#include <boost/config.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/random_number_generator.hpp>

// 2. advanced random generator providers have different names
static const std::string provider =
#ifdef BOOST_WINDOWS
  "Microsoft Strong Cryptographic Provider"
#else
  "/dev/urandom"
#endif
  ;

// 3. we are ready to initialize generator
boost::random_device device(provider);

// 4. let's get a uniform distribution
boost::random::uniform_int_distribution<unsigned short> random(1000);
