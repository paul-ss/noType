//
// Created by paul_s on 15.04.2020.
//
#include <boost/program_options.hpp>

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

//  boost::asio::io_service service;
//  auto rm = std::make_shared<RoomManager>();
//  auto db = std::make_shared<DataBaseFacade>();
//
//  Player p2("uuid2", "name2");
//  std::cout << rm->addPlayerAndRoom(p2, service, db) << std::endl;
//  Player p3("uuid3", "name1");
//  std::cout << rm->addPlayer(p3) << std::endl;
//  Player p4("uuid4", "name1");
//  std::cout << rm->addPlayer(p4) << std::endl;
//  Player p5("uuid5", "name1");
//  std::cout << rm->addPlayer(p5) << std::endl;
//  Player p7("uuid7", "name1");
//  if(auto res = rm->addPlayer(p7)) {
//    std::cout << res.value().playerID << "  " << res.value().waitTime  << std::endl;
//  } else {
//    std::cout << res.error().what() << std::endl;
//  }
//
//  std::thread interfaceThr([&]() {service.run();});

//  interfaceThr.join();


  Setup serverSetup;
  serverSetup.setup();
  serverSetup.start();


  return 0;
}