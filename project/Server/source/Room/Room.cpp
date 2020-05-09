//
// Created by paul_s on 16.04.2020.
//

#include "Room.hpp"
#include "RoomManager.hpp"


Room::Room(boost::asio::io_service &service,
             const std::shared_ptr<DataBaseFacade> &dataBaseFacade,
             const std::shared_ptr<RoomManager> &roomManager,
             const RoomConfig &roomConfig) :
    _timer(service),
    _dataBaseFacade(dataBaseFacade),
    _roomManager(roomManager),
    _roomConfig(roomConfig),
    _roomStatus(std::make_shared<RoomWait>(_roomConfig)),
    _text(""),
    _numberOfFinishers(0),
    _roomUUID(randomUUID()) {
  try {
    auto textInfo = _dataBaseFacade->GetRandomText();
    _text = textInfo->text;
  } catch (...) {
    _text = "Text default";
  }
}

Room::Room(boost::asio::io_service &service,
            const std::string &text,
            const std::shared_ptr<RoomManager> &roomManager,
            const RoomConfig &roomConfig) :
    _timer(service),
    _dataBaseFacade(std::make_shared<DataBaseFacade>()),
    _roomManager(roomManager),
    _roomConfig(roomConfig),
    _roomStatus(std::make_shared<RoomWait>(_roomConfig)),
    _text(text),
    _numberOfFinishers(0),
    _roomUUID(randomUUID()) {}
    



std::unordered_map<std::string, Player> Room::getPlayers() {
  std::unique_lock<std::mutex> lock(_roomMutex);
  return _players;
}





ExpectedRoom<AddPlayerResp> Room::addPlayer(const Player &player) {
  return _roomStatus->addPlayer(shared_from_this(), player);
}


ExpectedRoom<std::string> Room::getText() {
  return _roomStatus->getText(shared_from_this());
}


ExpectedRoom<size_t> Room::validateWrittenText(const std::string &text, const std::string &clientUUID) {
  return _roomStatus->validateWrittenText(shared_from_this(), text, clientUUID);
}


void Room::startAsyncEvent() {
  _roomStatus->startAsyncEvent(shared_from_this());
}

//void Room::deadlineHandler(const boost::system::error_code &ec) {
//
//}


std::string Room::getUUID() {
  return _roomUUID;
}

std::vector<std::string> Room::getPlayersUUID() {
  std::unique_lock<std::mutex> lock(_roomMutex);

  std::vector<std::string> result;
  for (auto &player : _players) {
    result.push_back(player.first);
  }

  return result;
}


void Room::setRoomStatus(const std::shared_ptr<IRoomStatus> &roomStatus) {
  std::unique_lock<std::mutex> lock(_roomMutex);
  _roomStatus = roomStatus;
}


std::string Room::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}



void Room::setPlayerState(const std::string &clientUUID, PlayerState state) {
  if (_players.count(clientUUID) == 0) {
    throw RoomException("setPlayerState : player with uuid " + clientUUID + " doesn't exist");
  }
  _players.at(clientUUID).state = state;
}




PlayerState Room::getPlayerState(const std::string &clientUUID) {
  if (_players.count(clientUUID) == 0) {
    throw RoomException("getPlayerState : player with uuid " + clientUUID + " doesn't exist");
  }
  return _players.at(clientUUID).state;
}




void Room::increaseTextPosition(const std::string &clientUUID, size_t increaseSize) {
  if (_players.count(clientUUID) == 0) {
    throw RoomException("increaseTextPosition : player with uuid " + clientUUID + " doesn't exist");
  }

  _players.at(clientUUID).textPosition += increaseSize;
}




size_t Room::getTextPosition(const std::string &clientUUID) {
  if (_players.count(clientUUID) == 0) {
    throw RoomException("getTextPosition : player with uuid " + clientUUID + " doesn't exist");
  }

  return _players.at(clientUUID).textPosition;
}



void Room::removeSelf() {
  auto roomManagerShared = _roomManager.lock();
  if (roomManagerShared) {
    if (!roomManagerShared->deleteRoom(_roomUUID)) {
      std::cout << "RemoveSelf error: can't erase room" << std::endl;
    }
    std::cout << "Room removed" << std::endl;
  } else {
    std::cout << "RemoveSelf error: can't make shared" << std::endl; //todo log
  }
}



void Room::sendStatistic() {
  for (auto &player : _players) {
    if (player.second.state == PLAYER_WIN) {
      updatePlayerInfo(player.second, 1, (int) player.second.textPosition / 3);
    } else if (player.second.state == PLAYER_FINISH) {
      updatePlayerInfo(player.second, 0, (int) player.second.textPosition / 4);
    } else {
      updatePlayerInfo(player.second, 0, (int) player.second.textPosition / 5);
    }
  }
}


void Room::updatePlayerInfo(const Player &player, int increaseWinsCount, int increasePoints) {
  try {
    auto findResult = _dataBaseFacade->FindPlayerInfoByUuid(player.clientUUID);

    if (findResult) {
      findResult->points += increasePoints;
      findResult->winsCount += increaseWinsCount;
      _dataBaseFacade->UpadatePlayerInfo(std::move(findResult));

    } else {
      auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(player.clientUUID, player.name);
      playerInfo->points += increasePoints;
      playerInfo->winsCount += increaseWinsCount;
      _dataBaseFacade->InsertPlayerInfo(std::move(playerInfo));

    }
  } catch (...) {
    std::cout << "Database exception" << std::endl;  // todo log
  }
}
