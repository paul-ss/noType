//
// Created by paul_s on 17.04.2020.
//

#pragma once


#include "CommandFactory.hpp"
#include "Queue.hpp"

#include <string>



class QueueManager {
public:
  virtual void serverPush(const std::string &data, const std::string &connectionUUID);
  void controllerPush(const std::shared_ptr<Command> &command);
  virtual bool serverPop(std::shared_ptr<Command> &command);
  bool basicControllerPop(std::shared_ptr<Command> &command);
  bool gameControllerPop(std::shared_ptr<Command> &command);
  void serverNotify();
private:
  Queue _queueToServer;
  Queue _queueToBasicController;
  Queue _queueToGameController;

  std::shared_ptr<CommandFactory> _commandFactory;
};

