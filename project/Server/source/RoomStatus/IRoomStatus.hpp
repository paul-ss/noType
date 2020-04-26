//
// Created by paul_s on 20.04.2020.
//

#pragma once

#include "Player.hpp"
#include "Expected.hpp"
#include "RoomConfig.hpp"


#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/pointer_cast.hpp>


class Room;
template<class T> using ExpectedRoom = Expected<T, RoomError>;


class IRoomStatus : public std::enable_shared_from_this<IRoomStatus> {
public:
  explicit IRoomStatus(const RoomConfig &roomConfig) :
      _roomConfig(roomConfig) {}
  virtual ~IRoomStatus() = default;
  virtual ExpectedRoom<bool> addPlayer(std::shared_ptr<Room> room, const Player &player) = 0;
  virtual ExpectedRoom<std::string> getText(std::shared_ptr<Room> room) = 0;
  virtual ExpectedRoom<size_t> validateWrittenText(std::shared_ptr<Room> room, const std::string &text, const std::string &clientUUID) = 0;
  virtual void startAsyncEvent(std::shared_ptr<Room> room) = 0; // doesn't catch mutex
  virtual void deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) = 0;

protected:
  RoomConfig _roomConfig;
};


