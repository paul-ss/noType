//
// Created by paul_s on 16.04.2020.
//

#include "GameController.hpp"


GameController::GameController(const std::shared_ptr<QueueManager> &queueManager,
                                const std::shared_ptr<IDataBaseFacade> &dataBaseFacade) :
    _queueManager(queueManager),
    _dataBaseFacade(dataBaseFacade),
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









// TODO catch all exceptions
void GameController::commandDistributor(const std::shared_ptr<Command> &command) {
  auto type = command->getTypeOfCommand();

  try {
    if (type == START_GAME_SESSION_REQUEST) {
      startGameSessionHandler(command);
    } else if (type == GET_TEXT_REQUEST) {
      getTextHandler(command);
    } else if (type == ROOM_STATUS_REQUEST) {
      getRoomStatusHandler(command);
    } else if (type == VALIDATE_WRITTEN_TEXT_REQUEST) {
      validateWrittenTextHandler(command);
    } else {
      std::cout << "GC : invalid command type" << std::endl;
      // todo log
    }

  } catch (ServerException &exc) {
    std::cout << "GC : exception : " << exc.what() << std::endl;
    // todo log
  } catch (...) {
    std::cout << "GC : exception : unknown" << std::endl;
  }
}



void GameController::startGameSessionHandler(const std::shared_ptr<Command> &command) {
  auto castedCmd = std::dynamic_pointer_cast<StartGameSessionRequest>(command);
  if (!castedCmd) {
    std::cout << "startGameSessionHandler : can't cast command" << std::endl;
    return;
    // todo log
  }

  // todo get player name
  Player player(castedCmd->key, "name");

  auto addPlayerRes = _roomManager->addPlayer(player);
  std::shared_ptr<StartGameSessionResponse> commandResp;

  if (!addPlayerRes) {
    auto addPlAndRoomRes = _roomManager->addPlayerAndRoom(player, _service, _dataBaseFacade);

    if (!addPlAndRoomRes) {
      std::cout << "startGameSessionHandler : can't add player. " << addPlAndRoomRes.error().what() << std::endl;
      // todo log
      commandResp = std::make_shared<StartGameSessionResponse>(
          castedCmd->getConnectionUUID(),
          fail,
          addPlAndRoomRes.error().what());
    }

    commandResp = std::make_shared<StartGameSessionResponse>(
        castedCmd->getConnectionUUID(),
        addPlAndRoomRes.value().playerID,
        addPlAndRoomRes.value().waitTime);

  } else {
    commandResp = std::make_shared<StartGameSessionResponse>(
        castedCmd->getConnectionUUID(),
        addPlayerRes.value().playerID,
        addPlayerRes.value().waitTime);
  }

  _queueManager->controllerPush(commandResp);
}


void GameController::getTextHandler(const std::shared_ptr<Command> &command) {
  auto castedCmd = std::dynamic_pointer_cast<GetTextRequest>(command);
  if (!castedCmd) {
    std::cout << "getTextHandler : can't cast command" << std::endl;
    return;
    // todo log
  }

  std::shared_ptr<GetTextResponse> commandResp;

  auto room = _roomManager->getRoom(castedCmd->key);
  if (!room) {
    commandResp = std::make_shared<GetTextResponse>(
        castedCmd->getConnectionUUID(),
        fail,
        "Player with uuid " + castedCmd->key + " doesn't exist in room");
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto textResp = room->getText();
  if (!textResp) {
    commandResp = std::make_shared<GetTextResponse>(
        castedCmd->getConnectionUUID(),
        fail,
        textResp.error().what());
    _queueManager->controllerPush(commandResp);
    return;
  }

  commandResp = std::make_shared<GetTextResponse>(castedCmd->getConnectionUUID(), textResp.value());
  _queueManager->controllerPush(commandResp);
}



void GameController::getRoomStatusHandler(const std::shared_ptr<Command> &command) {
  auto castedCmd = std::dynamic_pointer_cast<RoomStatusRequest>(command);
  if (!castedCmd) {
    std::cout << "getRoomStatusHandler : can't cast command" << std::endl;
    return;
    // todo log
  }

  std::shared_ptr<RoomStatusResponse> commandResp;

  auto room = _roomManager->getRoom(castedCmd->key);
  if (!room) {
    commandResp = std::make_shared<RoomStatusResponse>(
        castedCmd->getConnectionUUID(),
        fail,
        "Player with uuid " + castedCmd->key + " doesn't exist in room");
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto roomStatus = room->getRoomStatus();

  commandResp = std::make_shared<RoomStatusResponse>(
      castedCmd->getConnectionUUID(),
      roomStatus.state,
      0,
      roomStatus.players);

  _queueManager->controllerPush(commandResp);
}




void GameController::validateWrittenTextHandler(const std::shared_ptr<Command> &command) {
  auto castedCmd = std::dynamic_pointer_cast<ValidateWrittenTextRequest>(command);
  if (!castedCmd) {
    std::cout << "validateWrittenTextHandler : can't cast command" << std::endl;
    return;
    // todo log
  }

  std::shared_ptr<ValidateWrittenTextResponse> commandResp;

  auto room = _roomManager->getRoom(castedCmd->key);
  if (!room) {
    commandResp = std::make_shared<ValidateWrittenTextResponse>(
        castedCmd->getConnectionUUID(),
        fail,
        "Player with uuid " + castedCmd->key + " doesn't exist in room");
    _queueManager->controllerPush(commandResp);
    return;
  }

  auto validateResp = room->validateWrittenText(castedCmd->writtenText, castedCmd->key);
  if (!validateResp) {
    commandResp = std::make_shared<ValidateWrittenTextResponse>(
        castedCmd->getConnectionUUID(),
        fail,
        validateResp.error().what());
    _queueManager->controllerPush(commandResp);
    return;
  }

  commandResp = std::make_shared<ValidateWrittenTextResponse>(
      castedCmd->getConnectionUUID(),
      validateResp.value());

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

  std::cout << "QueueWorker (GC) stopped" << std::endl;
}

