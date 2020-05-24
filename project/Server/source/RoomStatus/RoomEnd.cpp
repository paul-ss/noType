//
// Created by paul_s on 27.04.2020.
//

#include "RoomEnd.hpp"

#include "Room.hpp"
#include "Logger.hpp"



RoomEnd::RoomEnd(const RoomConfig &roomConfig) :
    IRoomStatus(roomConfig) {}


ExpectedRoom<AddPlayerResp> RoomEnd::addPlayer(std::shared_ptr<Room> room, const Player &player) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return RoomError("addPlayer (END) : can't add player " + player.clientUUID);
}




bool RoomEnd::deletePlayer(std::shared_ptr<Room> room, const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);

  if (clientUUID.empty()) {
    throw RoomException("deletePlayer (END) : Invalid Player UUID at room " + room->_roomUUID);
  }

  if (room->_players.count(clientUUID) == 0) {
    throw RoomException("deletePlayer (END) : Attempt to delete not existing player at room " + room->_roomUUID);
  }

  room->sendOnePlayerStatistic(room->_players.at(clientUUID));

  if (room->_players.erase(clientUUID) == 0) {
    BOOST_LOG_TRIVIAL(error) << "deletePlayer (END) : Player disappeared somewhere. It's magic";
  }

  return true;
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
  return GetRoomStatusResp(room->_players, RoomState::end);
}



void RoomEnd::startAsyncEvent(std::shared_ptr<Room> room) {
  room->_timer.expires_from_now(std::chrono::milliseconds(_roomConfig._endDuration));
  room->_timer.async_wait(boost::bind(&RoomEnd::deadlineHandler, shared_from_this(), room, _1));
}


void RoomEnd::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec) {
    BOOST_LOG_TRIVIAL(error) << "RoomEnd handler error: " << ec.message();
    return;
  }

  BOOST_LOG_TRIVIAL(info) << "End handler";

  room->removeSelf();

  {
    std::unique_lock<std::mutex> lock(room->_roomMutex);
    room->sendStatistic();
  }
}



std::shared_ptr<RoomEnd> RoomEnd::shared_from_this() {
  return boost::static_pointer_cast<RoomEnd>(IRoomStatus::shared_from_this());
}
