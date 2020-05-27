//
// Created by paul_s on 13.05.2020.
//

#pragma once

#include "Command.hpp"

#include <string>



class ConnectRequest: public ClientCommand {
public:
  ConnectRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getClientUUID();

private:
  std::string _clientUUID;
};



class ConnectResponse: public ServerCommand {
public:
  explicit ConnectResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();
};