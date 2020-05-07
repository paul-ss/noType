//
// Created by paul_s on 07.05.2020.
//

#include "Setup.hpp"


Setup::~Setup() {
  for (auto &t : _threads) {
    t.join();
  }
}



void Setup::parseConfig() {

}


void Setup::start() {
  _tcpServer->startServer();
  _gameController->startController();
 // _basicController->startController();

  _threads.emplace_back(std::bind(&Setup::runInterface, this));
  std::cout << "Server started" << std::endl;
}


void Setup::stop() {
  _tcpServer->stopServer();
  _gameController->stopController();
  // _basicController->stopController();
}

void Setup::runInterface() {
  while (true) {
    std::string command;
    std::cout << "Enter command (s - stop) >>> " << std::endl;
    std::cin >> command;
    if (command == "s") {
      stop();
      break;
    } else {
      std::cout << "Invalid command: " + command << std::endl;
    }
  }
}


void Setup::setup() {
  // parse config
  _queueManager = std::make_unique<QueueManager>();
  // database
  _tcpServer = std::make_unique<TcpServer>(_queueManager);
  _gameController = std::make_unique<GameController>(_queueManager);
  _basicController = std::make_unique<BasicController>(/*_queueManager*/);
}
