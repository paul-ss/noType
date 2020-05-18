//
// Created by paul_s on 27.04.2020.
//

#include "RoomEnd.hpp"

#include "Room.hpp"



RoomEnd::RoomEnd(const RoomConfig &roomConfig) :
    IRoomStatus(roomConfig) {}


ExpectedRoom<AddPlayerResp> RoomEnd::addPlayer(std::shared_ptr<Room> room, const Player &player) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return RoomError("addPlayer (END) : can't add player " + player.clientUUID);
}


ExpectedRoom<std::string> RoomEnd::getText(std::shared_ptr<Room> room) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return RoomError("getText (END) : can't get text");
}


ExpectedRoom<size_t> RoomEnd::validateWrittenText(std::shared_ptr<Room> room,
                                                   const std::string &text,
                                                   const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return(RoomError("validateWrittenText (END) : Can't validate text.\n Sent data: UUID: " + clientUUID +
                   " text : " + text + "\nPlayers in this room: " + std::to_string(room->_players.size())));
}



GetRoomStatusResp RoomEnd::getRoomStatus(std::shared_ptr<Room> room) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);  // ??
  return GetRoomStatusResp(room->_players, ROOM_END);
}



void RoomEnd::startAsyncEvent(std::shared_ptr<Room> room) {
  room->_timer.expires_from_now(std::chrono::milliseconds(_roomConfig._endDuration));
  room->_timer.async_wait(boost::bind(&RoomEnd::deadlineHandler, shared_from_this(), room, _1));
}


void RoomEnd::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec) {
    std::cout << "RoomEnd handler error: " << ec.message() << std::endl;
    return;
  }
  std::cout << "End handler" << std::endl;

  {
    std::unique_lock<std::mutex> lock(room->_roomMutex);
    room->sendStatistic();
  }

  room->removeSelf();
}



std::shared_ptr<RoomEnd> RoomEnd::shared_from_this() {
  return boost::static_pointer_cast<RoomEnd>(IRoomStatus::shared_from_this());
}
