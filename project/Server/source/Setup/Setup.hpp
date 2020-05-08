//
// Created by paul_s on 07.05.2020.
//

#pragma once

#include "TcpServer.hpp"
#include "RoomManager.hpp"
#include "QueueManager.hpp"
#include "BasicController.hpp"
#include "GameController.hpp"
#include "DataBaseFacade.hpp"



class Setup {
public:

  ~Setup();
  void parseConfig();
  void start();
  void stop();
  void setup();

private:
  void runInterface();

private:
  std::shared_ptr<DataBaseFacade> _dataBaseFacade;
  std::shared_ptr<QueueManager> _queueManager;
  std::unique_ptr<BasicController> _basicController;
  std::unique_ptr<GameController> _gameController;
  std::unique_ptr<TcpServer> _tcpServer;

  std::vector<std::thread> _threads;
};

