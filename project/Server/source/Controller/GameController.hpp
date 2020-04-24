//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "QueueManager.hpp"
#include "RoomManager.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <unordered_map>
#include <thread>


enum GameControllerState {GAME_CONTROLLER_RUN, GAME_CONTROLLER_STOP};

class GameController {
public:

  GameController(std::shared_ptr<QueueManager> queueManager /*data : std::shared_ptr<Data>*/);
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


  std::shared_ptr<QueueManager> _queueManager ;
  //std::shared_ptr<Data> _data;
 // RoomManager _roomManager;
 // boost::asio::io_service _service; ??
  std::mutex _gameControllerMutex;
  GameControllerState _state;
  std::vector<std::thread> _threads;
};


