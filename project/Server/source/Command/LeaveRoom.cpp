//
// Created by paul_s on 24.05.2020.
//

#include "LeaveRoom.hpp"
#include "Logger.hpp"


LeaveRoomRequest::LeaveRoomRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::LeaveRoomRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void LeaveRoomRequest::parseFromPtree(pt::ptree &&pTree) {
  _clientUUID = pTree.get<std::string>(CLIENT_UUID_JSON_PATH);
}



std::string LeaveRoomRequest::getClientUUID() {
  return _clientUUID;
}










LeaveRoomResponse::LeaveRoomResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::LeaveRoomResponse, connectionUUID) {}


std::string LeaveRoomResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    BOOST_LOG_TRIVIAL(error) << "LeaveRoomResponse::parseToJSON :" + std::string(exc.what());
    auto resp = ErrorResponse(_connectionUUID,
                              "LeaveRoomResponse::parseToJSON :" + std::string(exc.what()));
    return resp.parseToJSON();


  } catch (...) {
    BOOST_LOG_TRIVIAL(error) << "LeaveRoomResponse::parseToJSON : unknown exception";
    auto resp = ErrorResponse(_connectionUUID,
                              "LeaveRoomResponse::parseToJSON : unknown exception");
    return resp.parseToJSON();
  }
}


std::string LeaveRoomResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}