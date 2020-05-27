//
// Created by paul_s on 13.05.2020.
//

#include "Init.hpp"


InitRequest::InitRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::InitRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void InitRequest::parseFromPtree(pt::ptree &&pTree) {
  _name = pTree.get<std::string>(CLIENT_NAME_JSON_PATH);
}



std::string InitRequest::getName() {
  return _name;
}






InitResponse::InitResponse(
        const std::string &connectionUUID,
        const std::string &clientUUID) :
    ServerCommand(CommandType::InitResponse, connectionUUID),
    _clientUUID(clientUUID) {}



InitResponse::InitResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::InitResponse, connectionUUID),
    _clientUUID("") {}


std::string InitResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    BOOST_LOG_TRIVIAL(error) << "InitResponse::parseToJSON :" + std::string(exc.what());
    auto resp = ErrorResponse(_connectionUUID,
                              "InitResponse::parseToJSON :" + std::string(exc.what()));
    return resp.parseToJSON();


  } catch (...) {
    BOOST_LOG_TRIVIAL(error) << "InitResponse::parseToJSON : unknown exception";
    auto resp = ErrorResponse(_connectionUUID,
                              "InitResponse::parseToJSON : unknown exception");
    return resp.parseToJSON();
  }
}


std::string InitResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);
  tree.put(CLIENT_UUID_JSON_PATH, _clientUUID);


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}