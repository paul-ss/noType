//
// Created by paul_s on 25.04.2020.
//

#pragma once

#include "IRoomStatus.hpp"
#include <memory>
#include <iostream>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

class RoomPlay : public IRoomStatus {
public:
  explicit RoomPlay(const RoomConfig &roomConfig);
  ExpectedRoom<AddPlayerResp> addPlayer(std::shared_ptr<Room> room, const Player &player) override;
  ExpectedRoom<std::string> getText(std::shared_ptr<Room> room) override;
  ExpectedRoom<size_t> validateWrittenText(std::shared_ptr<Room> room, const std::string &text, const std::string &clientUUID) override;
  GetRoomStatusResp getRoomStatus(std::shared_ptr<Room> room) override;
  void startAsyncEvent(std::shared_ptr<Room> room) override; // doesn't catch mutex!!
  void deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) override;

private:
  std::shared_ptr<RoomPlay> shared_from_this();
  size_t compareText(const std::string &roomText, const std::string &recvText, size_t playerPos);
  void calculatePlayersSpeed(std::shared_ptr<Room> room);

private:
  TimePoint _endGame;
  TimePoint _lastTimePlayersChecked;
};
