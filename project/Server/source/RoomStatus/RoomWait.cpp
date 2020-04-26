//
// Created by paul_s on 24.04.2020.
//

#include "RoomWait.hpp"
#include "RoomPlay.hpp"
#include "Room.hpp"



RoomWait::RoomWait(const RoomConfig &roomConfig) :
    IRoomStatus(roomConfig) {}


ExpectedRoom<bool> RoomWait::addPlayer(std::shared_ptr<Room> room, const Player &player) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);

  if (room->_players.size() >= _roomConfig._maxPlayersCount) {
    //throw
  }

  if (player._clientUUID.empty() ||  player._name.empty()) {
    //throw
  }

  room->_players.emplace(player._clientUUID, player);
  std::cout << "PLayer " << player._clientUUID << " added" << std::endl;

  if (room->_players.size() == 1) {
    room->startAsyncEvent();
  }

  if (room->_players.size() == _roomConfig._maxPlayersCount) {
    if (room->_timer.cancel() > 0) {
      room->_roomStatus = std::shared_ptr<IRoomStatus>(new RoomPlay(_roomConfig)); // todo RoomPlay
      room->startAsyncEvent();
    } else {
      // throw
    }
  }

  return true;
}


ExpectedRoom<std::string> RoomWait::getText(std::shared_ptr<Room> room) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return room->_text;
}


ExpectedRoom<size_t> RoomWait::validateWrittenText(std::shared_ptr<Room> room,
                                                    const std::string &text,
                                                    const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return(RoomError("Room status is 'WAIT'. Can't validate text.\n Sent data: UUID: " + clientUUID +
                    " text : " + text + "\nPlayers in this room: " + std::to_string(room->_players.size())));
}


void RoomWait::startAsyncEvent(std::shared_ptr<Room> room) {
  room->_timer.expires_from_now(boost::posix_time::milliseconds(_roomConfig._waitDuration));
  room->_timer.async_wait(boost::bind(&RoomWait::deadlineHandler, shared_from_this(), room, _1));
}


void RoomWait::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec /*&& ec != boost::asio::error::operation_aborted*/) {
    std::cout << "RoomWait handler error: " << ec.message() << std::endl;
    return;
  }
  std::cout << "handler" << std::endl;

  std::unique_lock<std::mutex> lock(room->_roomMutex);

  room->_roomStatus = std::shared_ptr<IRoomStatus>(new RoomPlay(_roomConfig)); // todo RoomPlay
  room->startAsyncEvent();
}



std::shared_ptr<RoomWait> RoomWait::shared_from_this() {
  return boost::static_pointer_cast<RoomWait>(IRoomStatus::shared_from_this());
}