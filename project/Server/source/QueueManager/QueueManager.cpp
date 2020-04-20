//
// Created by paul_s on 17.04.2020.
//

#include "QueueManager.hpp"


void QueueManager::serverPush(const std::string &data, const std::string &connectionUUID) {
  //std::unique_lock<std::mutex> lock(_queueToBasicControllerMutex);
  //std::unique_lock<std::mutex> lock1(_queneToGameControllerMutex);

  // todo remove
  auto command = std::make_shared<Command>(data, connectionUUID);
  std::unique_lock<std::mutex> lock(_queueToServerMutex);
  _queueToServer.push(command);
  _queueToServerCheck.notify_one();
}


void QueueManager::controllerPush(const std::shared_ptr<Command> &command) {
  std::unique_lock<std::mutex> lock(_queueToServerMutex);
  _queueToServer.push(command);
  _queueToServerCheck.notify_one();
}


bool QueueManager::serverPop(std::shared_ptr<Command> &command) {
  std::unique_lock<std::mutex> lock(_queueToServerMutex);
  if (_queueToServer.empty()) {
    _queueToServerCheck.wait(lock, [&]() { return !_queueToServer.empty(); });
  }

  if (!_queueToServer.empty()) {
    command = _queueToServer.front();
    _queueToServer.pop();
    return true;
  } else {
    return false;
  }
}


bool QueueManager::basicControllerPop(std::shared_ptr<Command> &command) {
  std::unique_lock<std::mutex> lock(_queueToBasicControllerMutex);
  return command->data.empty();
}


bool QueueManager::gameControllerPop(std::shared_ptr<Command> &command) {
  std::unique_lock<std::mutex> lock(_queueToGameControllerMutex);
  return command->data.empty();
}