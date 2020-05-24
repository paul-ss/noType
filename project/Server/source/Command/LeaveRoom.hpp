//
// Created by paul_s on 24.05.2020.
//

#pragma once

#include "Command.hpp"

#include <string>



class LeaveRoomRequest: public ClientCommand {
public:
  LeaveRoomRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getClientUUID();

private:
  std::string _clientUUID;
};



class LeaveRoomResponse: public ServerCommand {
public:
  LeaveRoomResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();
};