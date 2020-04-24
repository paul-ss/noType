//
// Created by paul_s on 20.04.2020.
//

#include "RoomManager.hpp"


#include <iostream>


RoomManager::RoomManager(/* Data */) {

}




bool RoomManager::deleteRoom(const std::string &roomUUID) {
  std::unique_lock<std::mutex> lock(_roomManagerMutex);
  if (_rooms.count(roomUUID) > 0) {
    // todo remove this
    auto playersUUID = _rooms[roomUUID]->getPlayersUUID();
    _rooms.erase(roomUUID);

    for (auto &playerUUID : playersUUID) {
      if (_players.count(playerUUID) > 0) {
        _players.erase(playerUUID);
      } else {
        throw  RoomManagerException("Player with UUID " + playerUUID + " exists in room, but not in RoomManager");
      }
    }

    return true;
  } else {
    return false;
  }
}



ExpectedRoom<bool> RoomManager::addPlayer(const Player &player) {
  std::unique_lock<std::mutex> lock(_roomManagerMutex);
  return addPlayerInternal(player);
}



bool RoomManager::addPlayerAndRoom(boost::asio::io_service &service,
                                    const std::string &text,
                                    const Player &player) {
  std::unique_lock<std::mutex> lock(_roomManagerMutex);

  if (auto addPlayerResult = addPlayerInternal(player)) {
    return true;
  }

  auto newRoomPtr = std::make_shared<Room>(service, text);
  auto playerAddRes = newRoomPtr->addPlayer(player);
  if (!playerAddRes) {
    throw RoomManagerException("Can't add player to new room. Room msg: " + playerAddRes.error().what());
  }

  _lastRoom = newRoomPtr;
  if (_rooms.count(newRoomPtr->getUUID()) > 0) {
    throw RoomManagerException("Room with UUID " + newRoomPtr->getUUID() + " already exists");
  }

  _rooms.emplace(newRoomPtr->getUUID(), newRoomPtr);
  _players.emplace(player._clientUUID, newRoomPtr->getUUID());

  return true;
}


std::shared_ptr<Room> RoomManager::getRoom(const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(_roomManagerMutex);
  if (_players.count(clientUUID) > 0) {
    auto roomUUID = _players[clientUUID];
    if (_rooms.count(roomUUID) > 0) {
      return _rooms[roomUUID];
    }
  }

  return std::shared_ptr<Room>();
}




ExpectedRoom<bool> RoomManager::addPlayerInternal(const Player &player) {
  if (player._clientUUID.empty()) {
    throw RoomManagerException("Invalid client UUID");
  }

  if (_players.count(player._clientUUID) > 0) {
    throw RoomManagerException("Player with UUID " + player._clientUUID + "already added to some room");
  }

  if (auto lastShared = _lastRoom.lock()) {
    if (auto addPlayerResult =  lastShared->addPlayer(player)) {
      // player added!
      _players.emplace(player._clientUUID, lastShared->getUUID());

      return true;
    } else {
      return addPlayerResult;
    }
  } else {
    return RoomError("Invalid last room");
  }
}



//bool RoomManager::deletePlayer(const std::string &playerUUID) {
//  if (_players.count(playerUUID) > 0) {
//    _players.erase(playerUUID);
//    return true;
//  }
//
//  return false;
//}



std::string RoomManager::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}