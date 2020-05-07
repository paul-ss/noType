//
// Created by paul_s on 15.04.2020.
//
#include "Setup.hpp"


//void interface(TcpServer &serv) {
//  while (1) {
//    std::string s;
//    std::cout << "type 's' to stop" << std::endl;
//    std::cin >> s;
//    if (s == "s") {
//      serv.stopServer();
//      std::cout << "stop" << std::endl;
//      break;
//    }
//  }
//}

int main() {
//  std::shared_ptr<QueueManager> q(new QueueManager);
//  TcpServer s(q, "127.0.0.1", 8000, 10);
//  std::thread interfaceThr(interface, std::ref(s));
//  s.startServer();

//  boost::asio::io_service service;
//  auto rm = std::make_shared<RoomManager>();
//
//  Player p2("uuid2", "name2");
//  std::cout << rm->addPlayerAndRoom(p2, service, "text") << std::endl;
//  Player p3("uuid3", "name1");
//  std::cout << rm->addPlayer(p3) << std::endl;
//  Player p4("uuid4", "name1");
//  std::cout << rm->addPlayer(p4) << std::endl;
//  Player p5("uuid5", "name1");
//  std::cout << rm->addPlayer(p5) << std::endl;
//  Player p1("uuid1", "name1");
//  std::cout << rm->addPlayer(p1) << std::endl;

//  Player p6("uuid6", "name1");
//  std::cout << rm->addPlayerAndRoom(service, "text", p6) << std::endl;
//  Player p7("uuid7", "name1");
//  if(auto res = rm->addPlayer(p7)) {
//    std::cout << res.value() << std::endl;
//  } else {
//    std::cout << res.error().what() << std::endl;
//  }

//  std::thread interfaceThr([&]() {service.run();});
//
////  std::cout << rm->deleteRoom(rm.getRoom("uuid2")->getUUID()) << std::endl;
////
////  std::cout << rm->addPlayer(p5) << std::endl;
//
//  interfaceThr.join();

  Setup serverSetup;
  serverSetup.setup();
  serverSetup.start();

  return 0;
}