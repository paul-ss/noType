//
// Created by paul_s on 15.04.2020.
//
#include "TcpServer.hpp"
#include <thread>


void interface(TcpServer &serv) {
  while (1) {
    std::string s;
    std::cout << "type 's' to stop" << std::endl;
    std::cin >> s;
    if (s == "s") {
      serv.stopServer();
      std::cout << "stop" << std::endl;
      break;
    }
  }
}

int main() {
  std::shared_ptr<QueueManager> q(new QueueManager);
  TcpServer s(q, "127.0.0.1", 8000, 10);
  std::thread interfaceThr(interface, std::ref(s));
  s.startServer();

  interfaceThr.join();
  return 0;
}