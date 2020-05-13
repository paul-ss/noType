//
// Created by paul_s on 13.05.2020.
//

#include "GetText.hpp"

GetTextRequest::GetTextRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::GetTextRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void GetTextRequest::parseFromPtree(pt::ptree &&pTree) {
  _clientUUID = pTree.get<std::string>(CLIENT_UUID_JSON_PATH);
}



std::string GetTextRequest::getClientUUID() {
  return _clientUUID;
}






GetTextResponse::GetTextResponse(
    const std::string &connectionUUID,
    const std::string &text) :
    ServerCommand(CommandType::GetTextResponse, connectionUUID),
    _text(text) {}



GetTextResponse::GetTextResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::GetTextResponse, connectionUUID),
    _text("") {}


std::string GetTextResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    std::cout << exc.what() << std::endl;
    // todo log
    // todo return valid json
    return "sorry";

  } catch (...) {
    std::cout << "GetTextResponse::parseToJSON : unknown exception" << std::endl;
    return "sorry";
  }
}


std::string GetTextResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);
  tree.put(ROOM_TEXT_JSON_PATH, _text);


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}