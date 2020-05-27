//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "QueueManager.hpp"
#include "RoomManager.hpp"
#include "DataBaseFacade.hpp"

#include "Init.hpp"
#include "Connect.hpp"
#include "GetRoomStatus.hpp"
#include "GetText.hpp"
#include "StartGameSession.hpp"
#include "ValidateWrittenText.hpp"
#include "LeaveRoom.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <thread>


enum GameControllerState {GAME_CONTROLLER_RUN, GAME_CONTROLLER_STOP};

class GameController {
public:

  GameController(const std::shared_ptr<QueueManager> &queueManager,
                  const std::shared_ptr<IDataBaseFacade> &dataBaseFacade,
                  RoomConfig &&roomConfig = RoomConfig());
  ~GameController();
  void startController();
  void stopController();


private:
  void commandDistributor(const std::shared_ptr<Command> &command);

  template <class RequestCommand, class ResponseCommand, typename CommandHandler>
  void handlerExceptionCatcher(const std::shared_ptr<Command> &command, CommandHandler handler);

  void startGameSessionHandler(const std::shared_ptr<StartGameSessionRequest> &command);
  void getTextHandler(const std::shared_ptr<GetTextRequest> &command);
  void getRoomStatusHandler(const std::shared_ptr<RoomStatusRequest> &command);
  void validateWrittenTextHandler(const std::shared_ptr<ValidateWrittenTextRequest> &command);
  void leaveRoomHandler(const std::shared_ptr<LeaveRoomRequest> &command);

  void runGameSessions();
  void runQueueWorker();


private:
  std::shared_ptr<QueueManager> _queueManager ;
  std::shared_ptr<IDataBaseFacade> _dataBaseFacade;
  RoomConfig _roomConfig;

  std::shared_ptr<RoomManager> _roomManager;
  boost::asio::io_service _service;
  boost::asio::io_service::work _work;
  std::mutex _gameControllerMutex;
  GameControllerState _state;
  std::vector<std::thread> _threads;
};
