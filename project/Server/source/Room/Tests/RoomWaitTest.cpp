//
// Created by paul_s on 04.05.2020.
//

#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"

void playersEQ(const Player &l, const Player &r);


class RoomWaitTest : public ::testing::Test {
protected:
  void SetUp() override {
    rc = RoomConfig(200, 5, 1000, 0, 0, 0);
    rm =  std::make_shared<RoomManager>();
    text = "text";
    room = std::make_shared<Room>(service, text, rm, rc);
  }

  void TearDown() override {}

  RoomConfig rc;
  boost::asio::io_service service;
  std::shared_ptr<RoomManager> rm;
  std::string text;
  std::shared_ptr<Room> room;
};




TEST_F(RoomWaitTest, add_player_) {
  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    auto addResp = room->addPlayer(player);
    ASSERT_TRUE(addResp);
    ASSERT_FALSE(addResp.value().playerID.empty());
    ASSERT_TRUE(addResp.value().waitTime <= 1000);
  }


  Player player1("uuid1", "name1" );
  ASSERT_THROW(room->addPlayer(player1), RoomException);

  Player player2("", "name1" );
  ASSERT_THROW(room->addPlayer(player1), RoomException);


  for (int i = 3; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    auto addResp = room->addPlayer(player);
    ASSERT_TRUE(addResp);
    ASSERT_FALSE(addResp.value().playerID.empty());
    ASSERT_TRUE(addResp.value().waitTime <= 1000);
  }

  Player player3("uuid7", "name1" );
  ASSERT_FALSE(room->addPlayer(player1));

  ASSERT_EQ(room->getPlayers().size(), 5);
}



TEST_F(RoomWaitTest, get_text) {
  auto recvt = room->getText();
  ASSERT_TRUE(recvt);
  ASSERT_EQ(text, recvt.value());
}


TEST_F(RoomWaitTest, validate_text) {
  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }

  ASSERT_FALSE(room->validateWrittenText("text", "uuid1"));
  ASSERT_FALSE(room->validateWrittenText("text", "u"));
  ASSERT_FALSE(room->validateWrittenText("", "uuid1"));
  ASSERT_FALSE(room->validateWrittenText("", ""));
}



TEST_F(RoomWaitTest, get_room_status) {
  std::vector<Player> pls;

  for (int i = 0; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
    pls.emplace_back(player);
  }

  auto res = room->getRoomStatus();
  ASSERT_EQ(res.state._to_string(), (+RoomState::wait)._to_string());

  ASSERT_EQ(res.players.size(), 5);

  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(res.players.count("uuid" + std::to_string(i)), 1);
    playersEQ(res.players.at("uuid" + std::to_string(i)), pls[i]);
  }
}


TEST_F(RoomWaitTest, delete_player) {
  std::vector<Player> pls;

  ASSERT_THROW(room->deletePlayer(""), RoomException);
  ASSERT_THROW(room->deletePlayer("uuid1"), RoomException);

  for (int i = 0; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
    pls.emplace_back(player);
  }

  ASSERT_THROW(room->deletePlayer("uuid5"), RoomException);
  ASSERT_THROW(room->deletePlayer(""), RoomException);

  for (int i = 0; i < 5; i++) {
    ASSERT_TRUE(room->deletePlayer(pls[i].clientUUID));
  }

  ASSERT_EQ(room->getPlayers().size(), 0);
  ASSERT_EQ(room->getPlayersUUID().size(), 0);
}


TEST_F(RoomWaitTest, delete_player_multithread) {
  std::vector<std::thread> thr;
  std::vector<Player> pls;

  for (int i = 0; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
    pls.emplace_back(player);
  }

  for (int i = 0; i < 5; i++) {
    thr.emplace_back([&, i] () {
      EXPECT_TRUE(room->deletePlayer(pls[i].clientUUID));
      EXPECT_THROW(room->deletePlayer(pls[i].clientUUID), RoomException);
    });
  }

  for (auto &t : thr) {
    t.join();
  }


  ASSERT_EQ(room->getPlayers().size(), 0);
  ASSERT_EQ(room->getPlayersUUID().size(), 0);
}


