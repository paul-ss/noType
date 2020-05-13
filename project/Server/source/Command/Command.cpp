//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#include "Command.hpp"


Command::Command() :
    _commandType(CommandType::ErrorRequest){}


Command::Command(CommandType commandType, const std::string &connectionUUID) :
    _commandType(commandType),
    _connectionUUID(connectionUUID) {}



CommandType Command::getCommandType() {
  return _commandType;
}



std::string Command::getConnectionUUID() {
  return _connectionUUID;
}






ClientCommand::ClientCommand() :
    _controllerType(ControllerType::basic) {}


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


ErrorResponse::ErrorResponse(const std::string &connectionUUID, std::string &&errorMsg) :
    ServerCommand(CommandType::ErrorResponse, connectionUUID) {
  setError(std::move(errorMsg));
}


std::string ErrorResponse::parseToJSON() {
  return "{\n \"" COMMAND_TYPE_JSON_PATH "\" : \"" + std::string(_commandType._to_string()) + "\",\n" +
         "\"" ERROR_MSG_JSON_PATH "\" : \"" + _errorMsg + "\",\n }";
}








ErrorRequest::ErrorRequest(const std::string &connectionUUID, const std::string &errorMsg) :
    ClientCommand(CommandType::ErrorRequest, connectionUUID),
    _errorMsg(errorMsg) {}


void ErrorRequest::parseFromPtree(pt::ptree &&pTree) {
  pTree.empty();
}


std::string ErrorRequest::getErrorMsg() {
  return _errorMsg;
}
