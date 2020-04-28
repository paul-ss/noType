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
    throw RoomException("addPlayer (WAIT) : Attempt to add player "
    + player._clientUUID + " to filled room " + room->_roomUUID);
  }

  if (player._clientUUID.empty() ||  player._name.empty()) {
    throw RoomException("addPlayer (WAIT) : Invalid Player UUID or name at room " + room->_roomUUID);
  }

  // TODO check insertion return value
  if (!room->_players.emplace(player._clientUUID, player).second) {
    throw RoomException("addPlayer (WAIT) : Can't add player " + room->_roomUUID);
  }
  std::cout << "PLayer " << player._clientUUID << " added" << std::endl;


  if (room->_players.size() == 1) {
    room->startAsyncEvent();
  }

  if (room->_players.size() == _roomConfig._maxPlayersCount) {
    if (room->_timer.cancel() > 0) {
      room->_roomStatus = std::shared_ptr<IRoomStatus>(std::make_shared<RoomPlay>(_roomConfig));
      room->startAsyncEvent();
    } else {
      //throw RoomException("addPlayer (WAIT) : No one async wait canceled. Maybe, that shouldn't be an exception.");
      std::cout << "addPlayer (WAIT) : No one async wait canceled in room " + room->_roomUUID << std::endl;
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
  return(RoomError("validateWrittenText (END) : Can't validate text.\n Sent data: UUID: " + clientUUID +
                    " text : " + text + "\nPlayers in this room: " + std::to_string(room->_players.size())));
}


void RoomWait::startAsyncEvent(std::shared_ptr<Room> room) {
  room->_timer.expires_from_now(std::chrono::milliseconds(_roomConfig._waitDuration));
  room->_timer.async_wait(boost::bind(&RoomWait::deadlineHandler, shared_from_this(), room, _1));
}


void RoomWait::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec /*&& ec != boost::asio::error::operation_aborted*/) {
    std::cout << "RoomWait handler error: " << ec.message() << std::endl;
    return;
  }
  std::cout << "handler" << std::endl;

  std::unique_lock<std::mutex> lock(room->_roomMutex);

  room->_roomStatus = std::shared_ptr<IRoomStatus>(std::make_shared<RoomPlay>(_roomConfig));
  room->startAsyncEvent();
}



std::shared_ptr<RoomWait> RoomWait::shared_from_this() {
  return boost::static_pointer_cast<RoomWait>(IRoomStatus::shared_from_this());
}


