//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "RoomWait.hpp"
#include "RoomPlay.hpp"
#include "RoomConfig.hpp"

#include <memory>
#include <mutex>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


template<class T> using ExpectedRoom = Expected<T, RoomError>;

enum RoomState {ROOM_WAIT, ROOM_PLAY, ROOM_END};

class Room : public std::enable_shared_from_this<Room> {
public:
  Room(boost::asio::io_service &service, const std::string &text);

  ExpectedRoom<bool> addPlayer(const Player &player);
  ExpectedRoom<std::string> getText();
  ExpectedRoom<size_t> validateWrittenText(const std::string &text, const std::string &clientUUID);
  void startAsyncEvent();
 // void deadlineHandler(const boost::system::error_code& error);

  std::string getUUID();
  std::vector<std::string> getPlayersUUID();

  friend class RoomWait;
  friend class RoomPlay;


private:
  //ExpectedRoom<RoomState> getStatus();
//  void transitionTo(std::shared_ptr<IRoomStatus> roomStatus);


  boost::asio::deadline_timer _timer;
 // RoomState _state;
  RoomConfig _roomConfig;
  std::shared_ptr<IRoomStatus> _roomStatus;
  std::unordered_map<std::string, Player> _players; // uuid, player
  std::string _text;
  size_t _finishLine;
  size_t _numberOfFinishers;
  size_t _maxPlayersCount;
  //- _timeFromStart : double
  std::string _roomUUID;
  std::mutex _roomMutex;
};


