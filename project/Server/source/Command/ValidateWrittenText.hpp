//
// Created by paul_s on 13.05.2020.
//

#pragma once

#include "Command.hpp"

#include <string>



class ValidateWrittenTextRequest: public ClientCommand {
public:
  ValidateWrittenTextRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getClientUUID();
  std::string getWrittenText();

private:
  std::string _clientUUID;
  std::string _writtenText;
};



class ValidateWrittenTextResponse: public ServerCommand {
public:
  ValidateWrittenTextResponse(const std::string &connectionUUID, size_t rightCount);
  ValidateWrittenTextResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();

private:
  size_t _rightCount;
};