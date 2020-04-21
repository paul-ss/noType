//
// Created by paul_s on 17.04.2020.
//

#include "QueueManager.hpp"


void QueueManager::serverPush(const std::string &data, const std::string &connectionUUID) {
  // command.factory ...
  // choose queue to push

  // todo remove
  auto command = std::make_shared<Command>(data, connectionUUID);
  _queueToServer.push(command);
}


void QueueManager::controllerPush(const std::shared_ptr<Command> &command) {
  _queueToServer.push(command);
}


bool QueueManager::serverPop(std::shared_ptr<Command> &command) {
  return _queueToServer.pop(command);
}


bool QueueManager::basicControllerPop(std::shared_ptr<Command> &command) {
  return _queueToBasicController.pop(command);
}


bool QueueManager::gameControllerPop(std::shared_ptr<Command> &command) {
  return _queueToGameController.pop(command);
}

void QueueManager::serverNotify() {
  _queueToServer.notify();
}