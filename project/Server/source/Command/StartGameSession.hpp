//
// Created by paul_s on 12.05.2020.
//

#pragma once

#include "Command.hpp"

#include <string>



class StartGameSessionRequest: public ClientCommand {
public:
  StartGameSessionRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getClientUUID();

private:
  std::string _clientUUID;
};



class StartGameSessionResponse: public ServerCommand {
public:
  StartGameSessionResponse(const std::string &connectionUUID, const std::string &playerID, unsigned int waitTime);
  StartGameSessionResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();

private:
  std::string _playerID;
  unsigned int _waitTime;
};