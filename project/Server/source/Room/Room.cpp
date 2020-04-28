//
// Created by paul_s on 16.04.2020.
//

#include "Room.hpp"


Room::  Room(boost::asio::io_service &service,
             const std::string &text,
             const std::shared_ptr<RoomManager> &roomManager,
             const RoomConfig &roomConfig) :
    _timer(service),
    _roomManager(roomManager),
    _roomConfig(roomConfig),
    _roomStatus(new RoomWait(_roomConfig)),
    _text(text),
    _numberOfFinishers(0),
    _roomUUID(randomUUID()){}

    //TODO use make shared !!



std::unordered_map<std::string, Player> Room::getPlayers() {
  std::unique_lock<std::mutex> lock(_roomMutex);
  return _players;
}





ExpectedRoom<bool> Room::addPlayer(const Player &player) {
  return _roomStatus->addPlayer(shared_from_this(), player);
}


ExpectedRoom<std::string> Room::getText() {
  return _roomStatus->getText(shared_from_this());
}


ExpectedRoom<size_t> Room::validateWrittenText(const std::string &text, const std::string &clientUUID) {
  return _roomStatus->validateWrittenText(shared_from_this(), text, clientUUID);
}


void Room::startAsyncEvent() {
  _roomStatus->startAsyncEvent(shared_from_this());
}

//void Room::deadlineHandler(const boost::system::error_code &ec) {
//
//}


std::string Room::getUUID() {
  return _roomUUID;
}

std::vector<std::string> Room::getPlayersUUID() {
  std::vector<std::string> result;
  for (auto &player : _players) {
    result.push_back(player.first);
  }

  return result;
}

std::string Room::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}