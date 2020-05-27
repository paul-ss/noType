//
// Created by paul_s on 27.04.2020.
//

#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"
#include <thread>


class RoomTest : public ::testing::Test {
protected:
  void SetUp() override {
    rc = RoomConfig(200, 5, 0, 0, 0, 0);
    rm =  std::make_shared<RoomManager>();
    text = "C++11 Examples: Contains a limited set of the C++03 Boost.Asio"
           "examples, updated to use only C++11 library and language facilities."
           "These examples do not make direct use of Boost C++ libraries."
           "C++17 Examples: Selected examples illustrating C++17 usage in conjunction with Technical Specifications.";
    room = std::make_shared<Room>(service, text, rm, rc);

    for (int i = 0; i < 5; i++) {
      Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
      pls.push_back(player);
      auto res = room->addPlayer(player);
      if (!res) {
        std::cout << res.error().what() << std::endl;
      }
      ASSERT_TRUE(res);
    }
  }

  void TearDown() override {}

  RoomConfig rc;
  boost::asio::io_service service;
  std::shared_ptr<RoomManager> rm;
  std::string text;
  std::shared_ptr<Room> room;
  std::vector<Player> pls;
};


void playersEQ(const Player &l, const Player &r) {
  ASSERT_EQ(l.clientUUID, r.clientUUID);
  ASSERT_EQ(l.name, r.name);
  ASSERT_EQ(l.textPosition, r.textPosition);
  ASSERT_EQ(l.lastTextPosition, r.lastTextPosition);
  ASSERT_EQ(l.playerID, r.playerID);
  ASSERT_EQ(l.state, r.state);
}


TEST_F(RoomTest, get_players) {
  auto players = room->getPlayers();
  ASSERT_EQ(players.size(), 5);

  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(players.count("uuid" + std::to_string(i)), 1);
    playersEQ(players.at("uuid" + std::to_string(i)), pls[i]);
  }
}



TEST_F(RoomTest, get_players_uuid) {
  auto uuids = room->getPlayersUUID();
  ASSERT_EQ(uuids.size(), 5);

  for (int i = 0; i < 5; i++) {
    ASSERT_TRUE(std::find(uuids.begin(), uuids.end(), pls[i].clientUUID) != uuids.end());
  }
}
