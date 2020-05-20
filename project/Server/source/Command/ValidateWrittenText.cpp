//
// Created by paul_s on 13.05.2020.
//

#include "ValidateWrittenText.hpp"
#include "Logger.hpp"

ValidateWrittenTextRequest::ValidateWrittenTextRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::ValidateWrittenTextRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void ValidateWrittenTextRequest::parseFromPtree(pt::ptree &&pTree) {
  _clientUUID = pTree.get<std::string>(CLIENT_UUID_JSON_PATH);
  _writtenText = pTree.get<std::string>(WRITTEN_TEXT_JSON_PATH);
}



std::string ValidateWrittenTextRequest::getClientUUID() {
  return _clientUUID;
}


std::string ValidateWrittenTextRequest::getWrittenText() {
  return _writtenText;
}





ValidateWrittenTextResponse::ValidateWrittenTextResponse(
        const std::string &connectionUUID,
        size_t rightCount) :
    ServerCommand(CommandType::ValidateWrittenTextResponse, connectionUUID),
    _rightCount(rightCount) {}



ValidateWrittenTextResponse::ValidateWrittenTextResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::ValidateWrittenTextResponse, connectionUUID),
    _rightCount(0) {}



std::string ValidateWrittenTextResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    //std::cout << "ValidateWrittenTextResponse::parseToJSON :" + std::string(exc.what()) << std::endl;
    BOOST_LOG_TRIVIAL(error) << "ValidateWrittenTextResponse::parseToJSON :" + std::string(exc.what());
    auto resp = ErrorResponse(_connectionUUID,
                              "ValidateWrittenTextResponse::parseToJSON :" + std::string(exc.what()));
    return resp.parseToJSON();


  } catch (...) {
    //std::cout << "ValidateWrittenTextResponse::parseToJSON : unknown exception" << std::endl;
    BOOST_LOG_TRIVIAL(error) << "ValidateWrittenTextResponse::parseToJSON : unknown exception";
    auto resp = ErrorResponse(_connectionUUID,
                              "ValidateWrittenTextResponse::parseToJSON : unknown exception");
    return resp.parseToJSON();
  }
}


std::string ValidateWrittenTextResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);
  tree.put(RIGHT_COUNT_JSON_PATH, _rightCount);


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}