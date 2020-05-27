//
// Created by paul_s on 25.04.2020.
//

#include "RoomPlay.hpp"
#include "RoomEnd.hpp"
#include "Room.hpp"
#include "Logger.hpp"



RoomPlay::RoomPlay(const RoomConfig &roomConfig) :
    IRoomStatus(roomConfig),
    _endGame(std::chrono::steady_clock::now() + std::chrono::milliseconds(_roomConfig._playDuration)),
    _lastTimePlayersChecked(std::chrono::steady_clock::now()) {}


ExpectedRoom<AddPlayerResp> RoomPlay::addPlayer(std::shared_ptr<Room> room, const Player &player) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);
  //std::cout << "PLayer " << player.clientUUID << " was not added" << std::endl;
  BOOST_LOG_TRIVIAL(info) << "PLayer " << player.clientUUID << " was not added";


  return RoomError("Room status is 'PLAY'. Can't add player with UUID " + player.clientUUID);
}


bool RoomPlay::deletePlayer(std::shared_ptr<Room> room, const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);

  if (clientUUID.empty()) {
    throw RoomException("deletePlayer (PLAY) : Invalid Player UUID at room " + room->_roomUUID);
  }

//  if (room->_players.erase(clientUUID) == 0) {
//    throw RoomException("deletePlayer (PLAY) : Attempt to delete not existing player at room " + room->_roomUUID);
//  }

//---- if player wants to send statistic, then room status is PLAY. But it takes much time.
  if (room->_players.count(clientUUID) == 0) {
    throw RoomException("deletePlayer (PLAY) : Attempt to delete not existing player at room " + room->_roomUUID);
  }

  room->sendOnePlayerStatistic(room->_players.at(clientUUID));

  if (room->_players.erase(clientUUID) == 0) {
    BOOST_LOG_TRIVIAL(error) << "deletePlayer (PLAY) : Player disappeared somewhere. It's magic";
  }
//----


  if (room->_players.size() == 0) {
    // all players gone, stop async handler
    if (room->_timer.cancel() > 0) {
      room->_roomStatus = std::shared_ptr<IRoomStatus>(std::make_shared<RoomEnd>(_roomConfig));
      room->startAsyncEvent();
      BOOST_LOG_TRIVIAL(info) << "deletePlayer (PLAY) : Async wait canceled in room " + room->_roomUUID + ". All players gone.";
    } else {
      BOOST_LOG_TRIVIAL(info) << "deletePlayer (PLAY) : No one async wait canceled in room " + room->_roomUUID;
    }
  }

  return true;
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
    throw RoomException("validateWrittenText (PLAY): Requested player " + clientUUID + " not found in room " + room->_roomUUID);
  }


  if (room->getPlayerState(clientUUID)._value != PlayerState::play) {
    return RoomError("Sorry, you can't enter the text, because you have already finished! Congratulations!");
  }

  size_t rightWritten = compareText(room->_text, recvText, room->getTextPosition(clientUUID));
  room->increaseTextPosition(clientUUID, rightWritten);

  auto textPosition = room->getTextPosition(clientUUID);
  if (textPosition >= room->_text.size() || textPosition >= _roomConfig._finishLine) {
    if (room->_numberOfFinishers == 0) {
      room->setPlayerState(clientUUID, PlayerState::win);
    } else {
      room->setPlayerState(clientUUID, PlayerState::finish);
    }
    room->_numberOfFinishers++;

    if (room->_numberOfFinishers == room->_players.size()) {
      // end game
      if (room->_timer.cancel() > 0) {
        room->_roomStatus = std::shared_ptr<IRoomStatus>(std::make_shared<RoomEnd>(_roomConfig));
        room->startAsyncEvent();
        BOOST_LOG_TRIVIAL(info) << "validateText (PLAY) : Async wait canceled in room " + room->_roomUUID
        + ". All players have already finished.";
      } else {
        BOOST_LOG_TRIVIAL(info) << "validateText (PLAY) : No one async wait canceled in room " + room->_roomUUID;
      }
    }
  }

  return rightWritten;
}



GetRoomStatusResp RoomPlay::getRoomStatus(std::shared_ptr<Room> room) {
  std::unique_lock<std::mutex> lock(room->_roomMutex);  // ??
  return GetRoomStatusResp(room->_players, RoomState::play);
}



void RoomPlay::startAsyncEvent(std::shared_ptr<Room> room) {
  if (std::chrono::steady_clock::now() + std::chrono::milliseconds(_roomConfig._playHandlerInterval) < _endGame) {
    room->_timer.expires_from_now(std::chrono::milliseconds(_roomConfig._playHandlerInterval));
  } else {
    auto now = std::chrono::steady_clock::now();

    if (_endGame > now) {
      room->_timer.expires_from_now(_endGame - now);
    } else {
      //TODO use make shared !!
      room->_roomStatus = std::shared_ptr<IRoomStatus>(std::make_shared<RoomEnd>(_roomConfig));
      room->startAsyncEvent();
      return;
    }
  }

  room->_timer.async_wait(boost::bind(&RoomPlay::deadlineHandler, shared_from_this(), room, _1));
}



void RoomPlay::deadlineHandler(std::shared_ptr<Room> room, const boost::system::error_code& ec) {
  if (ec) {
    BOOST_LOG_TRIVIAL(error) << "RoomPlay handler error: " << ec.message();
    return;
  }

  BOOST_LOG_TRIVIAL(debug) << "RoomPlay handler";

  std::unique_lock<std::mutex> lock(room->_roomMutex);
  calculatePlayersSpeed(room);
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


void RoomPlay::calculatePlayersSpeed(std::shared_ptr<Room> room) {
  std::chrono::duration<double, std::ratio<60>> timeDelta = std::chrono::steady_clock::now() - _lastTimePlayersChecked;
  for (auto &player : room->_players) {
    auto textDelta = player.second.textPosition - player.second.lastTextPosition;
    player.second.currentSpeed = textDelta / timeDelta.count();
    player.second.lastTextPosition = player.second.textPosition;
  }
  _lastTimePlayersChecked = std::chrono::steady_clock::now();
}