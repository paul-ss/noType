//
// Created by paul_s on 13.05.2020.
//

#include "GetRoomStatus.hpp"

RoomStatusRequest::RoomStatusRequest(const std::string &connectionUUID, pt::ptree &&pTree) :
    ClientCommand(CommandType::RoomStatusRequest, connectionUUID) {
  parseFromPtree(std::move(pTree));
}



void RoomStatusRequest::parseFromPtree(pt::ptree &&pTree) {
  _clientUUID = pTree.get<std::string>(CLIENT_UUID_JSON_PATH);
}



std::string RoomStatusRequest::getClientUUID() {
  return _clientUUID;
}






RoomStatusResponse::RoomStatusResponse(
        const std::string &connectionUUID,
        RoomStatus roomStatus,
        std::unordered_map<std::string, Player> &&players) :
    ServerCommand(CommandType::RoomStatusResponse, connectionUUID),
    _roomStatus(roomStatus),
    _players(std::move(players)) {}



RoomStatusResponse::RoomStatusResponse(const std::string &connectionUUID) :
    ServerCommand(CommandType::RoomStatusResponse, connectionUUID),
    _roomStatus(RoomStatus::wait) {}


std::string RoomStatusResponse::parseToJSON() {
  try {
    return internalParseToJSON();

  } catch (const pt::ptree_error &exc) {
    std::cout << exc.what() << std::endl;
    // todo log
    // todo return valid json
    return "sorry";

  } catch (...) {
    std::cout << "RoomStatusResponse::parseToJSON : unknown exception" << std::endl;
    return "sorry";
  }
}


std::string RoomStatusResponse::internalParseToJSON() {
  pt::ptree tree;

  tree.put(COMMAND_TYPE_JSON_PATH, _commandType._to_string());
  tree.put(RESPONSE_STATUS_JSON_PATH, _status._to_string());
  tree.put(ERROR_MSG_JSON_PATH, _errorMsg);
  tree.put(ROOM_STATUS_JSON_PATH, "roomStatus");

  for (auto &player : _players) {
    std::string playerCharacterDir(ROOM_PLAYERS_JSON_PATH "." + player.first + ".");

    tree.put(playerCharacterDir + PLAYER_NAME_JSON_DIR, player.second.name);
    tree.put(playerCharacterDir + PLAYER_POS_JSON_DIR, player.second.textPosition);
    tree.put(playerCharacterDir + PLAYER_SPEED_JSON_DIR, player.second.currentSpeed);
    tree.put(playerCharacterDir + PLAYER_STATUS_JSON_DIR, "playerStatus");
  }


  std::stringstream output_stream;
  pt::write_json(output_stream, tree);

  return output_stream.str();
}