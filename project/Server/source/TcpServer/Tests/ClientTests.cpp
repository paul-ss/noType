//
// Created by paul_s on 04.05.2020.
//

#include "gtest/gtest.h"
#include "Client.hpp"
#include "ConnectedClients.hpp"
#include "TcpServer.hpp"
#include <thread>

class FakeQueueManager : public QueueManager {
  void serverPush(const std::string &data, const std::string &connectionUUID) override {
    std::cout << data << " " << connectionUUID << std::endl;
  }
};


TEST(Client, what) {
  boost::asio::io_service service;
  auto clients = std::make_shared<ConnectedClients>();
  auto qm  = std::make_shared<FakeQueueManager>();
  auto c = std::make_shared<Client>(service, clients, qm, "uuid");

  std::vector<std::thread> thr;

  boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 10000);
  boost::asio::ip::tcp::acceptor a(service, ep);
  a.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  a.listen(1024);

  thr.emplace_back([&]() {
    a.accept(c->sock());
    c->read();
  });



  boost::asio::ip::tcp::socket sock(service);
  sock.open(boost::asio::ip::tcp::v4());
  sock.connect(ep);
  sock.write_some(boost::asio::buffer("123213123123jopa"));
  sock.close();

//  auto cmd = std::make_shared<Command>("data", "uuid");
//  qm->serverPop(cmd);
//  std::cout << cmd->data << std::endl;




  thr.emplace_back([&]() {service.run();});



  for (auto &t : thr) {
    t.join();
  }
}