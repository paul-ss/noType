//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#include "Command.hpp"
#define SUCCESS 0
#define FAIL 1


Command::Command(CommandType commandType, const std::string &connectionUUID) :
    _commandType(commandType),
    _connectionUUID(connectionUUID) {}



CommandType Command::getCommandType() {
  return _commandType;
}



std::string Command::getConnectionUUID() {
  return _connectionUUID;
}







ClientCommand::ClientCommand(CommandType commandType, const std::string &connectionUUID) :
    Command(commandType, connectionUUID),
    _controllerType(controllerTypeFromCommandType(commandType)) {}



ControllerType ClientCommand::getControllerType() {
  return _controllerType;
}


ControllerType ClientCommand::controllerTypeFromCommandType(CommandType commandType) {
  switch (commandType) {
    case (CommandType::InitRequest) :
      return ControllerType::basic;
    case (CommandType::ConnectRequest) :
      return ControllerType::basic;
    case (CommandType::ErrorRequest) :
      return ControllerType::basic;
    default:
      return ControllerType::game;
  }
}






ServerCommand::ServerCommand(CommandType commandType, const std::string &connectionUUID) :
    Command(commandType, connectionUUID),
    _status(ResponseStatus::success),
    _errorMsg("") {}


void ServerCommand::setError(std::string &&errorMsg) {
  _status = ResponseStatus::fail;
  _errorMsg = errorMsg;
}

