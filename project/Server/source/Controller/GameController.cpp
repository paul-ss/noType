//
// Created by paul_s on 16.04.2020.
//

#include "GameController.hpp"


GameController::GameController(const std::shared_ptr<QueueManager> &queueManager /*data : std::shared_ptr<Data>*/) :
    _queueManager(queueManager),
    //std::shared_ptr<Data> _data;
    _state(GAME_CONTROLLER_STOP) {}


GameController::~GameController() {
  for (auto &thread : _threads) {
    thread.join();
  }
}


void GameController::startController() {

}


void GameController::stopController() {

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
  while(1) {
    {
      std::unique_lock<std::mutex> lock(_gameControllerMutex);
      if (_state == GAME_CONTROLLER_STOP) {
        break;
      }
    }

    std::shared_ptr<Command> command;
    if (_queueManager->serverPop(command)) {
      commandDistributor(command);
    }
  }
}

