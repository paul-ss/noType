//
// Created by paul_s on 17.04.2020.
//

#include "QueueManager.hpp"

QueueManager::QueueManager() :
    _queueToServer(std::make_shared<Queue>()),
    _queueToBasicController(std::make_shared<Queue>()),
    _queueToGameController(std::make_shared<Queue>()),
    _commandFactory(std::make_shared<CommandFactory>()) {}


QueueManager::QueueManager(const std::shared_ptr<Queue> &qToServer,
                            const std::shared_ptr<Queue> &qToBasic,
                            const std::shared_ptr<Queue> &qToGame,
                            const std::shared_ptr<CommandFactory> &commandFactory) :
    _queueToServer(qToServer),
    _queueToBasicController(qToBasic),
    _queueToGameController(qToGame),
    _commandFactory(commandFactory) {}

void QueueManager::serverPush(const std::string &data, const std::string &connectionUUID) {
  // command.factory ...
  // choose queue to push
  // TODO !!!! catch exceptions

  auto command = _commandFactory->createCommand(connectionUUID, data);
  if (command->getControllerType() == ControllerType::game) {
    _queueToGameController->push(command);
  } else {
    _queueToBasicController->push(command);
  }
}


void QueueManager::controllerPush(const std::shared_ptr<Command> &command) {
  _queueToServer->push(command);
}


bool QueueManager::serverPop(std::shared_ptr<Command> &command) {
  return _queueToServer->pop(command);
}


bool QueueManager::basicControllerPop(std::shared_ptr<Command> &command) {
  return _queueToBasicController->pop(command);
}


bool QueueManager::gameControllerPop(std::shared_ptr<Command> &command) {
  return _queueToGameController->pop(command);
}

void QueueManager::serverNotify() {
  _queueToServer->notify();
}

void QueueManager::gameControllerNotify() {
  _queueToGameController->notify();
}

void QueueManager::basicControllerNotify() {
  _queueToBasicController->notify();
}