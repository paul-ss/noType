//
// Created by paul_s on 13.05.2020.
//

#pragma once

#include "Command.hpp"

#include <string>



class RoomStatusRequest: public ClientCommand {
public:
  RoomStatusRequest(const std::string &connectionUUID, pt::ptree &&pTree);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getClientUUID();

private:
  std::string _clientUUID;
};



class RoomStatusResponse: public ServerCommand {
public:
  RoomStatusResponse(
        const std::string &connectionUUID,
        RoomStatus roomStatus,
        std::unordered_map<std::string, Player> &&players);
  RoomStatusResponse(const std::string &connectionUUID);
  std::string parseToJSON() override;

private:
  std::string internalParseToJSON();

private:
  RoomStatus _roomStatus;
  std::unordered_map<std::string, Player> _players;
};