//
// Created by paul_s on 17.04.2020.
//

#pragma once

#include "Command.hpp"
#include "CommandFactory.hpp"

#include <string>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>


class QueueManager {
public:
  void serverPush(const std::string &data, const std::string &connectionUUID);
  void controllerPush(const std::shared_ptr<Command> &command);
  bool serverPop(std::shared_ptr<Command> &command);
  bool basicControllerPop(std::shared_ptr<Command> &command);
  bool gameControllerPop(std::shared_ptr<Command> &command);
private:
  std::queue <std::shared_ptr<Command>> _queueToServer;
  std::queue <std::shared_ptr<Command>> _queueToBasicController;
  std::queue <std::shared_ptr<Command>> _queueToGameController;

  std::mutex _queueToServerMutex;
  std::mutex _queueToBasicControllerMutex;
  std::mutex _queueToGameControllerMutex;

  std::condition_variable _queueToServerCheck;
  std::shared_ptr<CommandFactory> _commandFactory;
};

