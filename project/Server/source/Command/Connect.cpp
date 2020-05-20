//
// Created by paul_s on 13.05.2020.
//

#include "Connect.hpp"
#include "Logger.hpp"

ConnectRequest::ConnectRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::ConnectRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void ConnectRequest::parseFromPtree(pt::ptree &&pTree) {
  _clientUUID = pTree.get<std::string>(CLIENT_UUID_JSON_PATH);
}



std::string ConnectRequest::getClientUUID() {
  return _clientUUID;
}







ConnectResponse::ConnectResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::ConnectResponse, connectionUUID) {}


std::string ConnectResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    //std::cout << "ConnectResponse::parseToJSON :" + std::string(exc.what()) << std::endl;
    BOOST_LOG_TRIVIAL(debug) << "ConnectResponse::parseToJSON :" + std::string(exc.what());
    
    auto resp = ErrorResponse(_connectionUUID,
        "ConnectResponse::parseToJSON :" + std::string(exc.what()));
    return resp.parseToJSON();


  } catch (...) {
    //std::cout << "ConnectResponse::parseToJSON : unknown exception" << std::endl;
    BOOST_LOG_TRIVIAL(error) << "ConnectResponse::parseToJSON : unknown exception";
    auto resp = ErrorResponse(_connectionUUID,
                              "ConnectResponse::parseToJSON : unknown exception");
    return resp.parseToJSON();
  }
}


std::string ConnectResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}