//
// Created by paul_s on 04.05.2020.
//

#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"
#include "FakeDataBase.hpp"

#include <thread>

void playersEQ(const Player &l, const Player &r);

class RoomPlayTest : public ::testing::Test {
protected:
  void SetUp() override {
    rc = RoomConfig(200, 5, 0, 0, 0, 0);
    rm =  std::make_shared<RoomManager>();
    text = "C++11 Examples: Contains a limited set of the C++03 Boost.Asio"
           "examples, updated to use only C++11 library and language facilities."
           "These examples do not make direct use of Boost C++ libraries."
           "C++17 Examples: Selected examples illustrating C++17 usage in conjunction with Technical Specifications.";
    db = std::make_shared<FakeDataBase>(text);
    room = std::make_shared<Room>(service, db, rm, rc);

    for (int i = 0; i < 5; i++) {
      Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
      auto res = room->addPlayer(player);
      if (!res) {
        std::cout << res.error().what() << std::endl;
      }
      ASSERT_TRUE(res);
      pls.emplace_back(player);
    }

    room->setRoomStatus(std::make_shared<RoomPlay>(RoomPlay(rc)));
  }

  void TearDown() override {}

  RoomConfig rc;
  boost::asio::io_service service;
  std::shared_ptr<RoomManager> rm;
  std::shared_ptr<IDataBaseFacade> db;
  std::string text;
  std::shared_ptr<Room> room;
  std::vector<Player> pls;
};



TEST_F(RoomPlayTest, validate_text) {
  ASSERT_THROW(room->validateWrittenText("text", "123"), RoomException);
  ASSERT_THROW(room->validateWrittenText("text", ""), RoomException);
  ASSERT_THROW(room->validateWrittenText("", "123"), RoomException);

  auto recv = room->validateWrittenText("text", "uuid2");
  ASSERT_TRUE(recv);
  ASSERT_EQ(recv.value(), 0);


  size_t pos1 = 0;
  while(auto res = room->validateWrittenText(text.substr(pos1, 20), "uuid0")) {
    pos1 += res.value();
  }

  size_t pos2 = 0;
  while(auto res = room->validateWrittenText(text.substr(pos2, 17), "uuid1")) {
    pos2 += res.value();
  }


  auto players = room->getPlayers();


  ASSERT_EQ(players.at("uuid0").textPosition, 200);
  ASSERT_EQ(players.at("uuid0").state._value, PlayerState::win);
  ASSERT_GT(players.at("uuid1").textPosition, 200);
  ASSERT_EQ(players.at("uuid1").state._value, PlayerState::finish);
  ASSERT_EQ(players.at("uuid2").textPosition, 0);
  ASSERT_EQ(players.at("uuid2").state._value, PlayerState::play);
}



TEST_F(RoomPlayTest, validate_text_multithread) {
  std::vector<std::thread> thr;

  for (int i = 0; i < 5; i++) {
    thr.emplace_back([&, i]() {
      size_t pos = 0;
      for (int j = 0; j < i * 5; j++) {
        auto res = room->validateWrittenText(text.substr(pos, i), "uuid" + std::to_string(i));
        pos += res.value();
      }
    });
  }

  for (auto &t : thr) {
    t.join();
  }

  auto players = room->getPlayers();
  ASSERT_EQ(players.size(), 5);

  for (int i = 1; i < 5; i++) {
    ASSERT_EQ(players.at("uuid" + std::to_string(i)).textPosition, i * i * 5);
  }
}




TEST_F(RoomPlayTest, add_player) {
  ASSERT_FALSE(room->addPlayer(Player("uu", "name")));
  ASSERT_FALSE(room->addPlayer(Player("uuid1", "name")));
  ASSERT_FALSE(room->addPlayer(Player("", "name")));
  ASSERT_FALSE(room->addPlayer(Player("", "")));
}


TEST_F(RoomPlayTest, get_text) {
  auto recvt = room->getText();
  ASSERT_TRUE(recvt);
  ASSERT_EQ(text, recvt.value());
}


TEST_F(RoomPlayTest, get_room_status) {
  auto res = room->getRoomStatus();
  ASSERT_EQ(res.state._to_string(), (+RoomState::play)._to_string());

  ASSERT_EQ(res.players.size(), 5);

  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(res.players.count("uuid" + std::to_string(i)), 1);
    playersEQ(res.players.at("uuid" + std::to_string(i)), pls[i]);
  }
}



TEST_F(RoomPlayTest, delete_player) {
  ASSERT_THROW(room->deletePlayer("uuid5"), RoomException);
  ASSERT_THROW(room->deletePlayer(""), RoomException);

  for (int i = 0; i < 5; i++) {
    ASSERT_TRUE(room->deletePlayer(pls[i].clientUUID));
  }

  ASSERT_EQ(room->getPlayers().size(), 0);
  ASSERT_EQ(room->getPlayersUUID().size(), 0);
}



TEST_F(RoomPlayTest, delete_player_multithread) {
  std::vector<std::thread> thr;

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
