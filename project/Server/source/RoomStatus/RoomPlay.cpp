//
// Created by paul_s on 25.04.2020.
//

#include "RoomPlay.hpp"
#include "Room.hpp"



RoomPlay::RoomPlay(const RoomConfig &roomConfig) :
    IRoomStatus(roomConfig) {}


ExpectedRoom<bool> RoomPlay::addPlayer(std::shared_ptr<Room> room, const Player &player) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  std::cout << "PLayer " << player._clientUUID << " was not added" << std::endl;
  return RoomError("Room status is 'PLAY'. Can't add player with UUID " + player._clientUUID);
}



ExpectedRoom<std::string> RoomPlay::getText(std::shared_ptr<Room> room) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  return room->_text;  // player may do it, but why?
}



ExpectedRoom<size_t> RoomPlay::validateWrittenText(std::shared_ptr<Room> room,
                                                   const std::string &recvText,
                                                   const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);

  if (room->_players.count(clientUUID) == 0) {
    // todo throw player not found
  }


  if (room->_players.at(clientUUID)._state != PLAYER_PLAY) {
    return RoomError("Sorry, you can't enter the text, because you have already finished! Congratulations!");
  }

  size_t rightWritten = compareText(room->_text, recvText, room->_players.at(clientUUID)._textPosition);
  room->_players.at(clientUUID)._textPosition += rightWritten;

  if (room->_players.at(clientUUID)._textPosition >= room->_text.size()) {
    if (room->_numberOfFinishers == 0) {
      room->_players.at(clientUUID)._state = PLAYER_WIN;
    } else {
      room->_players.at(clientUUID)._state = PLAYER_FINISH;
    }
    room->_numberOfFinishers++;

    if (room->_numberOfFinishers == room->_players.size()) {
      //todo end of game, change room state
    }
  }

  return rightWritten;
}


void RoomPlay::startAsyncEvent(std::shared_ptr<Room> room) {
  room->_timer.expires_from_now(boost::posix_time::milliseconds(_roomConfig._playHandlerInterval));
  room->_timer.async_wait(boost::bind(&RoomPlay::deadlineHandler, shared_from_this(), room, _1));
}


void RoomPlay::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec /*&& ec != boost::asio::error::operation_aborted*/) {
    std::cout << "RoomPlay handler error: " << ec.message() << std::endl;
    return;
  }
  std::cout << "RoomPlay handler" << std::endl;

  std::unique_lock<std::mutex> lock(room->_roomMutex);

  // todo calculate players speed
//  if () {
//    room->_roomStatus = std::shared_ptr<IRoomStatus>(new RoomPlay(_roomConfig)); // todo RoomEnd
//  }
  room->startAsyncEvent();
}



std::shared_ptr<RoomPlay> RoomPlay::shared_from_this() {
  return boost::static_pointer_cast<RoomPlay>(IRoomStatus::shared_from_this());
}


size_t RoomPlay::compareText(const std::string &roomText, const std::string &recvText, size_t playerPos) {
  size_t recvSize = recvText.size();
  size_t roomTextSize = roomText.size();

  size_t i = 0;
  for (; i < recvSize && i + playerPos < roomTextSize; i++) {
    if (recvText[i] != roomText[i + playerPos]) {
      break;
    }
  }

  return i;
}