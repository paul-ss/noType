//
// Created by paul_s on 16.04.2020.
//

#include "GameController.hpp"
#include "Logger.hpp"


GameController::GameController(const std::shared_ptr<QueueManager> &queueManager,
                                const std::shared_ptr<IDataBaseFacade> &dataBaseFacade,
                                RoomConfig &&roomConfig) :
    _queueManager(queueManager),
    _dataBaseFacade(dataBaseFacade),
    _roomConfig(std::move(roomConfig)),
    _roomManager(std::make_shared<RoomManager>()),
    _work(_service),
    _state(GAME_CONTROLLER_STOP) {}


GameController::~GameController() {
  for (auto &thread : _threads) {
    thread.join();
  }
}


void GameController::startController() {
  std::unique_lock<std::mutex> lock(_gameControllerMutex);
  for (size_t i = 0; i < 1; i++) {
    _threads.emplace_back(std::bind(&GameController::runGameSessions, this));
  }

  for (size_t i = 0; i < 1; i++) {
    _threads.emplace_back(std::bind(&GameController::runQueueWorker, this));
  }

  _state = GAME_CONTROLLER_RUN;
}


void GameController::stopController() {
  std::unique_lock<std::mutex> lock(_gameControllerMutex);
  _service.stop();
  _state = GAME_CONTROLLER_STOP;
  _queueManager->gameControllerNotify();
}





void GameController::commandDistributor(const std::shared_ptr<Command> &command) {
  try {
    auto type = command->getCommandType();

    switch (type) {
      case (CommandType::StartGameSessionRequest) :
        handlerExceptionCatcher<StartGameSessionRequest, StartGameSessionResponse>(
            command,
            boost::bind(&GameController::startGameSessionHandler, this, _1));
        break;

      case (CommandType::GetTextRequest) :
        handlerExceptionCatcher<GetTextRequest, GetTextResponse>(
            command,
            boost::bind(&GameController::getTextHandler, this, _1));
        break;

      case (CommandType::RoomStatusRequest) :
        handlerExceptionCatcher<RoomStatusRequest, RoomStatusResponse>(
            command,
            boost::bind(&GameController::getRoomStatusHandler, this, _1));
        break;

      case (CommandType::ValidateWrittenTextRequest) :
        handlerExceptionCatcher<ValidateWrittenTextRequest, ValidateWrittenTextResponse>(
            command,
            boost::bind(&GameController::validateWrittenTextHandler, this, _1));
        break;

      case (CommandType::LeaveRoomRequest) :
        handlerExceptionCatcher<LeaveRoomRequest, LeaveRoomResponse>(
            command,
            boost::bind(&GameController::leaveRoomHandler, this, _1));
        break;

      default:
        BOOST_LOG_TRIVIAL(error) << "GC : invalid command type";

        auto commandResp = std::make_shared<ErrorResponse>(
            command->getConnectionUUID(),
            "GC : invalid command type");
        _queueManager->controllerPush(commandResp);
    }

  } catch (...) {
    BOOST_LOG_TRIVIAL(error) << "GC : exception : unknown, unexpected, mysterious... (like James Bond)";

    auto commandResp = std::make_shared<ErrorResponse>(
        command->getConnectionUUID(),
        "GC : exception : unknown, unexpected, mysterious... (like James Bond)" );
    _queueManager->controllerPush(commandResp);
  }
}





template <class RequestCommand, class ResponseCommand, typename CommandHandler>
void GameController::handlerExceptionCatcher(const std::shared_ptr<Command> &command, CommandHandler handler) {
  auto castedCmd = std::dynamic_pointer_cast<RequestCommand>(command);
  if (!castedCmd) {
    BOOST_LOG_TRIVIAL(error) << "Can't cast command " + std::string(typeid(command).name())
    << "handler type: " + std::string(typeid(handler).name());

    auto commandResp = std::make_shared<ErrorResponse>(
        castedCmd->getConnectionUUID(),
        "Can't cast command " + std::string(typeid(command).name()));
    _queueManager->controllerPush(commandResp);

    return;
  }

  try {
    handler(castedCmd);

  } catch (std::exception &exc) {
    auto commandResp = std::make_shared<ResponseCommand>(castedCmd->getConnectionUUID());
    commandResp->setError(exc.what());
    _queueManager->controllerPush(commandResp);

    BOOST_LOG_TRIVIAL(error) << "GC : exception : " << exc.what();

  } catch (...) {
    auto commandResp = std::make_shared<ResponseCommand>(castedCmd->getConnectionUUID());
    commandResp->setError("Exception : unknown exception type");
    _queueManager->controllerPush(commandResp);

    BOOST_LOG_TRIVIAL(error) << "GC : exception : unknown exception type";
  }
}







void GameController::startGameSessionHandler(const std::shared_ptr<StartGameSessionRequest> &command) {
  auto playerInfo = _dataBaseFacade->FindPlayerInfoByUuid(command->getClientUUID());
  if (playerInfo == nullptr) {
    auto commandResp = std::make_shared<StartGameSessionResponse>(command->getConnectionUUID());

    commandResp->setError("startGameSessionHandler : Can't find player in DB. Try 'init' or 'connect'");
    BOOST_LOG_TRIVIAL(info) << "startGameSessionHandler : Can't find player in DB.";

    _queueManager->controllerPush(commandResp);
    return;
  }

  Player player(command->getClientUUID(), playerInfo->name);

  auto addPlayerRes = _roomManager->addPlayer(player);
  std::shared_ptr<StartGameSessionResponse> commandResp;

  if (!addPlayerRes) {
    auto addPlAndRoomRes = _roomManager->addPlayerAndRoom(player, _service, _dataBaseFacade, _roomConfig);

    if (!addPlAndRoomRes) {
      throw GameControllerException("startGameSessionHandler : can't add player. " + addPlAndRoomRes.error().what());
    }

    commandResp = std::make_shared<StartGameSessionResponse>(
        command->getConnectionUUID(),
        addPlAndRoomRes.value().playerID,
        addPlAndRoomRes.value().waitTime);

  } else {
    commandResp = std::make_shared<StartGameSessionResponse>(
        command->getConnectionUUID(),
        addPlayerRes.value().playerID,
        addPlayerRes.value().waitTime);
  }

  _queueManager->controllerPush(commandResp);
}





void GameController::getTextHandler(const std::shared_ptr<GetTextRequest> &command) {
  auto room = _roomManager->getRoom(command->getClientUUID());
  if (!room) {
    auto commandResp = std::make_shared<GetTextResponse>(command->getConnectionUUID());
    commandResp->setError("Player with uuid " + command->getClientUUID() + " doesn't exist in room");
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto textResp = room->getText();
  if (!textResp) {
    auto commandResp = std::make_shared<GetTextResponse>(command->getConnectionUUID());
    commandResp->setError(textResp.error().what());
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto commandResp = std::make_shared<GetTextResponse>(command->getConnectionUUID(), textResp.value());
  _queueManager->controllerPush(commandResp);
}






void GameController::getRoomStatusHandler(const std::shared_ptr<RoomStatusRequest> &command) {
  auto room = _roomManager->getRoom(command->getClientUUID());
  if (!room) {
    auto commandResp = std::make_shared<RoomStatusResponse>(command->getConnectionUUID());
    commandResp->setError("Player with uuid " + command->getClientUUID() + " doesn't exist in room");
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto roomStatus = room->getRoomStatus();

  auto commandResp = std::make_shared<RoomStatusResponse>(
      command->getConnectionUUID(),
      roomStatus.state,
      std::move(roomStatus.players));

  _queueManager->controllerPush(commandResp);
}




void GameController::validateWrittenTextHandler(const std::shared_ptr<ValidateWrittenTextRequest> &command) {
  auto room = _roomManager->getRoom(command->getClientUUID());
  if (!room) {
    auto commandResp = std::make_shared<ValidateWrittenTextResponse>(command->getConnectionUUID());
    commandResp->setError("Player with uuid " + command->getClientUUID() + " doesn't exist in room");
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto validateResp = room->validateWrittenText(command->getWrittenText(), command->getClientUUID());
  if (!validateResp) {
    auto commandResp = std::make_shared<ValidateWrittenTextResponse>(command->getConnectionUUID());
    commandResp->setError(validateResp.error().what());
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto commandResp = std::make_shared<ValidateWrittenTextResponse>(
      command->getConnectionUUID(),
      validateResp.value());

  _queueManager->controllerPush(commandResp);
}





void GameController::leaveRoomHandler(const std::shared_ptr<LeaveRoomRequest> &command) {
  if (!_roomManager->deletePlayer(command->getClientUUID())) {
    BOOST_LOG_TRIVIAL(info) << "GC : Can't delete player. Player doesn't exist in RM";
  }

  auto commandResp = std::make_shared<LeaveRoomResponse>(command->getConnectionUUID());
  _queueManager->controllerPush(commandResp);
}








void GameController::runGameSessions() {
  _service.run();
}


void GameController::runQueueWorker() {
  while(true) {
    {
      std::unique_lock<std::mutex> lock(_gameControllerMutex);
      if (_state == GAME_CONTROLLER_STOP) {
        break;
      }
    }

    std::shared_ptr<Command> command;
    if (_queueManager->gameControllerPop(command)) {
      commandDistributor(command);
    }
  }

  BOOST_LOG_TRIVIAL(info) << "QueueWorker (GC) stopped";
}

