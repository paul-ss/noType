//
// Created by paul_s on 16.04.2020.
//

#include "Room.hpp"


Room::Room(boost::asio::io_service &service, const std::string &text) :
    _timer(service),
    _roomStatus(new RoomWait(_roomConfig)),
    _text(text),
    _finishLine(200),
    _numberOfFinishers(0),
    _maxPlayersCount(5) {}



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