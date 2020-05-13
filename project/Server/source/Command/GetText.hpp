//
// Created by paul_s on 13.05.2020.
//


#pragma once

#include "Command.hpp"

#include <string>



class GetTextRequest: public ClientCommand {
public:
  GetTextRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getClientUUID();

private:
  std::string _clientUUID;
};



class GetTextResponse: public ServerCommand {
public:
  GetTextResponse(const std::string &connectionUUID, const std::string &text);
  GetTextResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();

private:
  std::string _text;
};
