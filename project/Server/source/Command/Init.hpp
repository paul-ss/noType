//
// Created by paul_s on 13.05.2020.
//

#pragma once

#include "Command.hpp"

#include <string>



class InitRequest: public ClientCommand {
public:
  InitRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getName();

private:
  std::string _name;
};



class InitResponse: public ServerCommand {
public:
  InitResponse(const std::string &connectionUUID, const std::string &clientUUID);
  InitResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();

private:
  std::string _clientUUID;
};