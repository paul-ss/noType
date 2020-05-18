//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "RoomWait.hpp"
#include "RoomPlay.hpp"
#include "RoomConfig.hpp"
#include "DataBaseFacade.hpp"

#include <memory>

#include <mutex>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>


template<class T> using ExpectedRoom = Expected<T, RoomError>;
class RoomManager;


class Room : public std::enable_shared_from_this<Room> {
public:
  Room(boost::asio::io_service &service,
       const std::shared_ptr<IDataBaseFacade> &dataBaseFacade,
        const std::shared_ptr<RoomManager> &roomManager,
        const RoomConfig &roomConfig = RoomConfig());

  // for tests
  Room(boost::asio::io_service &service,
       const std::string &text,
       const std::shared_ptr<RoomManager> &roomManager,
       const RoomConfig &roomConfig = RoomConfig());

  std::unordered_map<std::string, Player> getPlayers();
  ExpectedRoom<AddPlayerResp> addPlayer(const Player &player);
  ExpectedRoom<std::string> getText();
  ExpectedRoom<size_t> validateWrittenText(const std::string &text, const std::string &clientUUID);
  GetRoomStatusResp getRoomStatus();
  void startAsyncEvent();


  std::string getUUID();
  std::vector<std::string> getPlayersUUID();
  void setRoomStatus(const std::shared_ptr<IRoomStatus> &roomStatus);

  friend class RoomWait;
  friend class RoomPlay;
  friend class RoomEnd;

private:
  //ExpectedRoom<RoomState> getStatus();
//  void transitionTo(std::shared_ptr<IRoomStatus> roomStatus);
  std::string randomUUID();
  void setPlayerState(const std::string &clientUUID, PlayerState state);
  PlayerState getPlayerState(const std::string &clientUUID);
  void increaseTextPosition(const std::string &clientUUID, size_t increaseSize);
  size_t getTextPosition(const std::string &clientUUID);

  void removeSelf();
  void sendStatistic();
  void updatePlayerInfo(const Player &player, int increaseWinsCount, int increasePoints);


private:
  boost::asio::steady_timer _timer;
  std::shared_ptr<IDataBaseFacade> _dataBaseFacade;
  std::weak_ptr<RoomManager> _roomManager;

  RoomConfig _roomConfig;
  std::shared_ptr<IRoomStatus> _roomStatus;
  std::unordered_map<std::string, Player> _players; // uuid, player
  std::string _text;
  size_t _numberOfFinishers;
  //- _timeFromStart : double
  std::string _roomUUID;
  std::mutex _roomMutex;
};


