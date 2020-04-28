//
// Created by paul_s on 20.04.2020.
//

#pragma once

#include "Room.hpp"
#include "Player.hpp"
#include "Exceptions.hpp"

#include <string>
#include <unordered_map>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>


class RoomManager : public std::enable_shared_from_this<RoomManager> {
public:
  RoomManager(/* Data */);
 // void addPlayer(const Player &player, boost::asio::io_service &service);

  bool deleteRoom(const std::string &roomUUID);
  ExpectedRoom<bool> addPlayer(const Player &player);
  bool addPlayerAndRoom(const Player &player,
                         boost::asio::io_service &service,
                         const std::string &text,
                         const RoomConfig &roomConfig = RoomConfig());
  std::shared_ptr<Room> getRoom(const std::string &clientUUID);



private:
  ExpectedRoom<bool> addPlayerInternal(const Player &player); // doesn't catch mutex!
  bool deletePlayer(const std::string &playerUUID); // doesn't catch mutex!
  std::string randomUUID();




  std::weak_ptr<Room> _lastRoom;
  std::unordered_map<std::string, std::shared_ptr<Room>> _rooms;
  std::unordered_map<std::string, std::string> _players; // <clientUUID, roomId>
  //boost::asio::io_service _service;
  std::mutex _roomManagerMutex;
};


