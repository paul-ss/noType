//
// Created by paul_s on 17.04.2020.
//

#pragma once


#include "CommandFactory.hpp"
#include "Queue.hpp"

#include <string>



class QueueManager {
public:
  QueueManager();
  QueueManager(const std::shared_ptr<Queue> &qToServer,
               const std::shared_ptr<Queue> &qToBasic,
               const std::shared_ptr<Queue> &qToGame);
  virtual void serverPush(const std::string &data, const std::string &connectionUUID);
  void controllerPush(const std::shared_ptr<Command> &command);
  virtual bool serverPop(std::shared_ptr<Command> &command);
  bool basicControllerPop(std::shared_ptr<Command> &command);
  bool gameControllerPop(std::shared_ptr<Command> &command);
  void serverNotify();


private:
  std::shared_ptr<Queue> _queueToServer;
  std::shared_ptr<Queue> _queueToBasicController;
  std::shared_ptr<Queue> _queueToGameController;

  std::shared_ptr<CommandFactory> _commandFactory;
};

