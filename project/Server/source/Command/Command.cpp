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


ErrorResponse::ErrorResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::ErrorResponse, connectionUUID) {}


std::string ErrorResponse::parseToJSON() {
  boost::property_tree::ptree pt;
  pt.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  pt.put(ERROR_MSG_JSON_PATH, _errorMsg);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
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
