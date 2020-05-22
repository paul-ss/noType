//
// Created by paul_s on 15.04.2020.
//
#include <boost/program_options.hpp>
#include <boost/asio/signal_set.hpp>

#include "Setup.hpp"
#include "Logger.hpp"



int main(int argc, const char *argv[]) {
  try {
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help screen")
      ("trace","TRACE")
      ("debug","DEBUG")
      ("info", "INFO")
      ("warning","WARNING")
      ("error","ERROR")
      ("fatal", "FATAL");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    } else if (vm.count("trace")) {
      initLogger(boost::log::trivial::trace);
    } else if (vm.count("debug")) {
     // initLogger(boost::log::trivial::debug);
    } else if (vm.count("info")) {
      initLogger(boost::log::trivial::info);
    } else if (vm.count("warning")) {
      initLogger(boost::log::trivial::warning);
    } else if (vm.count("error")) {
      initLogger(boost::log::trivial::error);
    } else if (vm.count("fatal")) {
      initLogger(boost::log::trivial::fatal);
    } else {
      std::cout << desc << std::endl;
      return EXIT_FAILURE;
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << std::endl;
  }


  Setup serverSetup;

  serverSetup.setup();
  serverSetup.start();



  return 0;
}
