//
// Created by paul_s on 15.04.2020.
//
#include "Setup.hpp"


int main() {


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