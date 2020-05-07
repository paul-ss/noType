//
// Created by paul_s on 04.05.2020.
//

#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"


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
    ASSERT_TRUE(room->addPlayer(player));
  }


  Player player1("uuid1", "name1" );
  ASSERT_THROW(room->addPlayer(player1), RoomException);

  Player player2("", "name1" );
  ASSERT_THROW(room->addPlayer(player1), RoomException);


  for (int i = 3; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }

  Player player3("uuid7", "name1" );
  ASSERT_FALSE(room->addPlayer(player1));
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