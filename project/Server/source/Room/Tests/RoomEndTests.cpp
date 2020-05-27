//
// Created by paul_s on 24.05.2020.
//

#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"
#include "FakeDataBase.hpp"
#include "RoomEnd.hpp"

#include <thread>

void playersEQ(const Player &l, const Player &r);

class RoomEndTest : public ::testing::Test {
protected:
  void SetUp() override {
    rc = RoomConfig(200, 5, 0, 0, 0, 0);
    rm =  std::make_shared<RoomManager>();
    text = "text";
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

    room->setRoomStatus(std::make_shared<RoomEnd>(RoomEnd(rc)));
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


TEST_F(RoomEndTest, delete_player) {
  ASSERT_THROW(room->deletePlayer("uuid5"), RoomException);
  ASSERT_THROW(room->deletePlayer(""), RoomException);

  for (int i = 0; i < 5; i++) {
    ASSERT_TRUE(room->deletePlayer(pls[i].clientUUID));
  }

  ASSERT_EQ(room->getPlayers().size(), 0);
  ASSERT_EQ(room->getPlayersUUID().size(), 0);
}



TEST_F(RoomEndTest, delete_player_multithread) {
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