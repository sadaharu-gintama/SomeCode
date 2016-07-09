#include <boost/program_options.hpp>
// 'read_file' exception class is declared in errors.hpp
#include <boost/program_options/errors.hpp>
#include <iostream>
namespace opt = boost::program_options;

int main(int argc, char *argv[]) {
  opt::options_description desc("All Options:");
  // Adding options to option list
  desc.add_options()
    ("apple,a", opt::value<int>()->default_value(10),
     "Apples that you have")
    ("orange,o", opt::value<int>()->default_value(10), "Oranges that you have")
    ("name", opt::value<std::string>(), "Your name")
    ("help", "Produce help messages")
    ;

  opt::variables_map vm;
  // Parsing command line options and storing data in 'vm'
  opt::store(opt::parse_command_line(argc, argv, desc), vm);
  // We can also parse environment variables using 'parse_environment' method
  opt::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }
  // Adding missing options from "apple_oranges.cfg"
  try {
    opt::store(opt::parse_config_file<char>("apple_oranges.cfg", desc), vm);
  }
  catch (const opt::reading_file &e) {
    std::cout << "Failed to open file 'apple_oranges.cfg': "
              << e.what() << std::endl;
  }

  opt::notify(vm);
  if (vm.count("name")) {
    std::cout << "Hi, " << vm["name"].as<std::string>() << "!" << std::endl;
  }

  std::cout << "Fruit count: "
            << vm["apple"].as<int>() + vm["orange"].as<int>() << std::endl;
  return 0;
}
