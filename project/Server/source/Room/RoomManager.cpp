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



ExpectedRoom<AddPlayerResp> RoomManager::addPlayer(const Player &player) {
  std::unique_lock<std::mutex> lock(_roomManagerMutex);
  return addPlayerInternal(player);
}



ExpectedRoom<AddPlayerResp> RoomManager::addPlayerAndRoom(const Player &player,
                                                          boost::asio::io_service &service,
                                                          const std::shared_ptr<DataBaseFacade> &dataBaseFacade,
                                                          const RoomConfig &roomConfig) {
  std::unique_lock<std::mutex> lock(_roomManagerMutex);

  auto addPlayerResult = addPlayerInternal(player);
  if (addPlayerResult) {
    return addPlayerResult;
  }


  auto newRoomPtr = std::make_shared<Room>(service, dataBaseFacade, shared_from_this(), roomConfig);

  auto playerAddResNewRoom = newRoomPtr->addPlayer(player);
  if (!playerAddResNewRoom) {
    throw RoomManagerException("Can't add player to new room. Room msg: " + playerAddResNewRoom.error().what());
  }

  _lastRoom = newRoomPtr;
  if (_rooms.count(newRoomPtr->getUUID()) > 0) {
    throw RoomManagerException("Room with UUID " + newRoomPtr->getUUID() + " already exists");
  }

  auto roomEmplaceRes = _rooms.emplace(newRoomPtr->getUUID(), newRoomPtr);
  auto playerEmplaceRes = _players.emplace(player.clientUUID, newRoomPtr->getUUID());
  if (!roomEmplaceRes.second || !playerEmplaceRes.second) {
    _rooms.erase(newRoomPtr->getUUID());
    _players.erase(player.clientUUID);
    throw RoomManagerException("Cant emplace Room or Player");
  }

  return playerAddResNewRoom;
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




ExpectedRoom<AddPlayerResp> RoomManager::addPlayerInternal(const Player &player) {
  if (player.clientUUID.empty()) {
    throw RoomManagerException("Invalid client UUID");
  }

  if (_players.count(player.clientUUID) > 0) {
    throw RoomManagerException("Player with UUID " + player.clientUUID + "already added to some room");
  }

  auto lastShared = _lastRoom.lock();
  if (lastShared) {
    auto addPlayerResult =  lastShared->addPlayer(player);
    if (addPlayerResult) {
      // player added!
      auto emplaceRes = _players.emplace(player.clientUUID, lastShared->getUUID());
      if (!emplaceRes.second) {
        // todo delete player ??
        throw RoomManagerException("Player with UUID " + player.clientUUID + "can't be emplaced in '_players'");
      }
    }

    return addPlayerResult;

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