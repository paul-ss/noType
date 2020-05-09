//
// Created by paul_s on 16.04.2020.
//

#include "GameController.hpp"


GameController::GameController(const std::shared_ptr<QueueManager> &queueManager,
                                const std::shared_ptr<IDataBaseFacade> &dataBaseFacade) :
    _queueManager(queueManager),
    _dataBaseFacade(dataBaseFacade),
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
  command->data.size();
}


void GameController::startGameSessionHandler(const std::shared_ptr<Command> &command) {
  command->data.size();
}


void GameController::getTextHandler(const std::shared_ptr<Command> &command) {
  command->data.size();
}


void GameController::getRoomStatusHandler(const std::shared_ptr<Command> &command) {
  command->data.size();
}


void GameController::sendWrittenTextHandler(const std::shared_ptr<Command> &command) {
  command->data.size();
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

