//
// Created by paul_s on 12.05.2020.
//

#include "StartGameSession.hpp"



StartGameSessionRequest::StartGameSessionRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::StartGameSessionRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void StartGameSessionRequest::parseFromPtree(pt::ptree &&pTree) {
  _clientUUID = pTree.get<std::string>(CLIENT_UUID_JSON_PATH);
}



std::string StartGameSessionRequest::getClientUUID() {
  return _clientUUID;
}






StartGameSessionResponse::StartGameSessionResponse(
          const std::string &connectionUUID,
          const std::string &playerID,
          unsigned int waitTime) :
    ServerCommand(CommandType::StartGameSessionResponse, connectionUUID),
    _playerID(playerID),
    _waitTime(waitTime) {}



StartGameSessionResponse::StartGameSessionResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::StartGameSessionResponse, connectionUUID),
    _playerID(""),
    _waitTime(0) {}


std::string StartGameSessionResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    std::cout << "StartGameSessionResponse::parseToJSON :" + std::string(exc.what()) << std::endl;
    auto resp = ErrorResponse(_connectionUUID,
                              "StartGameSessionResponse::parseToJSON :" + std::string(exc.what()));
    return resp.parseToJSON();


  } catch (...) {
    std::cout << "StartGameSessionResponse::parseToJSON : unknown exception" << std::endl;
    auto resp = ErrorResponse(_connectionUUID,
                              "StartGameSessionResponse::parseToJSON : unknown exception");
    return resp.parseToJSON();
  }
}


std::string StartGameSessionResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);
  tree.put(PLAYER_ID_JSON_PATH, _playerID);
  tree.put(WAIT_TIME_JSON_PATH, _waitTime);


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}


