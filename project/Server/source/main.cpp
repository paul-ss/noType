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
      std::cout << desc << '\n';
    } else if (vm.count("trace")) {
      initLogger(boost::log::trivial::trace);
    } else if (vm.count("debug")) {
      initLogger(boost::log::trivial::debug);
    } else if (vm.count("info")) {
      initLogger(boost::log::trivial::info);
    } else if (vm.count("warning")) {
      initLogger(boost::log::trivial::warning);
    } else if (vm.count("error")) {
      initLogger(boost::log::trivial::error);
    } else if (vm.count("fatal")) {
      initLogger(boost::log::trivial::fatal);
    }

  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << '\n';
  }

  Setup serverSetup;

  boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
  signals.async_wait([&](const boost::system::error_code& error, int signal_number) {
    const boost::system::error_code& error,
    int signal_number) {
      if (!error) {
      serverSetup.stop();
  });

  serverSetup.setup();
  serverSetup.start();

  return 0;
}
