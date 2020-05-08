//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "QueueManager.hpp"
#include "RoomManager.hpp"
#include "DataBaseFacade/DataBaseFacade.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <thread>


enum GameControllerState {GAME_CONTROLLER_RUN, GAME_CONTROLLER_STOP};

class GameController {
public:

  GameController(const std::shared_ptr<QueueManager> &queueManager,
                  const std::shared_ptr<DataBaseFacade> &dataBaseFacade);
  ~GameController();
  void startController();
  void stopController();


private:
  void commandDistributor(const std::shared_ptr<Command> &command);
  void startGameSessionHandler(const std::shared_ptr<Command> &command);
  void getTextHandler(const std::shared_ptr<Command> &command);
  void getRoomStatusHandler(const std::shared_ptr<Command> &command);
  void sendWrittenTextHandler(const std::shared_ptr<Command> &command);

  void runGameSessions();
  void runQueueWorker();


private:
  std::shared_ptr<QueueManager> _queueManager ;
  std::shared_ptr<DataBaseFacade> _dataBaseFacade;

  RoomManager _roomManager;
  boost::asio::io_service _service;
  boost::asio::io_service::work _work;
  std::mutex _gameControllerMutex;
  GameControllerState _state;
  std::vector<std::thread> _threads;
};


