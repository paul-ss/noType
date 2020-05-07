//
// Created by paul_s on 27.04.2020.
//

#pragma once

#include "IRoomStatus.hpp"
#include "Exceptions.hpp"

#include <memory>
#include <iostream>


class RoomEnd : public IRoomStatus {
public:
  explicit RoomEnd(const RoomConfig &roomConfig);
  ExpectedRoom<bool> addPlayer(std::shared_ptr<Room> room, const Player &player) override;
  ExpectedRoom<std::string> getText(std::shared_ptr<Room> room) override;
  ExpectedRoom<size_t> validateWrittenText(std::shared_ptr<Room> room, const std::string &text, const std::string &clientUUID) override;
  void startAsyncEvent(std::shared_ptr<Room> room) override; // doesn't catch mutex!!
  void deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) override;

private:
  std::shared_ptr<RoomEnd> shared_from_this();
};


