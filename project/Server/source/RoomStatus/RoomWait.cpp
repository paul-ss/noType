//
// Created by paul_s on 24.04.2020.
//

#include "RoomWait.hpp"
#include "Room.hpp"

ExpectedRoom<bool> RoomWait::addPlayer(std::shared_ptr<Room> room, const Player &player) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);

  if (room->_players.size() >= room->_maxPlayersCount) {
    //throw
  }

  if (player._clientUUID.empty() ||  player._clientUUID.empty()) {
    //throw
  }

  room->_players.emplace(player._clientUUID, player);

  if (room->_players.size() == 1) {
    room->startAsyncEvent();
  }

  if (room->_players.size() == room->_maxPlayersCount) {
    room->_timer.cancel();
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
  return(RoomError("Room status is 'WAIT'. Can't validate text.\n Sent data: UUID: " + clientUUID +
                    " text : " + text + "\nPlayers in this room: " + std::to_string(room->_players.size())));
}


void RoomWait::startAsyncEvent(std::shared_ptr<Room> room) {
  room->_timer.expires_from_now(boost::posix_time::seconds(5));
  room->_timer.async_wait(boost::bind(&RoomWait::deadlineHandler, shared_from_this(), room, _1));
}


void RoomWait::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec && ec != boost::asio::error::operation_aborted) {
    std::cout << "handler error" << std::endl;
    return;
  }
  std::cout << "handler" << std::endl;

  std::unique_lock<std::mutex> lock(room->_roomMutex);

  room->_roomStatus = std::shared_ptr<IRoomStatus>(new RoomWait()); // todo RoomPlay
  room->startAsyncEvent();
}

