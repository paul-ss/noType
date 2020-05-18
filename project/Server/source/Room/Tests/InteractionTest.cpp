//
// Created by paul_s on 10.05.2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Room.hpp"
#include "RoomManager.hpp"

#include <thread>

using ::testing::Return;
using ::testing::ByMove;
using ::testing::_;
using DataBase::External::TextInfo;
using DataBase::External::PlayerInfo;

class MockDB : public IDataBaseFacade {
  void UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) override {
    UpadatePlayerInfo_(std::move(plyerInfo));
  }
  void InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) override {
    InsertPlayerInfo_(std::move(plyerInfo));
  }

public:
  MOCK_METHOD1(InsertPlayerInfo_, void(const std::shared_ptr<DataBase::External::PlayerInfo> &info));
  MOCK_METHOD1(UpadatePlayerInfo_, void(const std::shared_ptr<DataBase::External::PlayerInfo> &info));
  MOCK_METHOD1(FindPlayerInfoByUuid, std::unique_ptr<DataBase::External::PlayerInfo>(const std::string& uuid));
  MOCK_METHOD0(GetRandomText, std::unique_ptr<DataBase::External::TextInfo>());
};


class Interaction : public ::testing::Test {
protected:
  void SetUp() override {
    rc = RoomConfig(200, 5, 500, 500, 500, 500);
    rm =  std::make_shared<RoomManager>();
    text = "C++11 Examples: Contains a limited set of the C++03 Boost.Asio"
           "examples, updated to use only C++11 library and language facilities."
           "These examples do not make direct use of Boost C++ libraries."
           "C++17 Examples: Selected examples illustrating C++17 usage in conjunction with Technical Specifications.";
    db = std::make_shared<MockDB>();

    auto textInfo = std::make_unique<TextInfo>(0, std::string(text));
    EXPECT_CALL(*db, GetRandomText())
        .WillOnce(Return(ByMove(std::move(textInfo))));
    room = std::make_shared<Room>(service, db, rm, rc);

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
  std::shared_ptr<MockDB> db;
  std::string text;
  std::shared_ptr<Room> room;
  std::vector<Player> pls;
};



TEST_F(Interaction, get_random_text) {
  auto textInfo = std::make_unique<TextInfo>(0, std::string(text));
  EXPECT_CALL(*db, GetRandomText())
  .WillOnce(Return(ByMove(std::move(textInfo))));
  auto room1 = std::make_shared<Room>(service, db, rm, rc);

  auto textResp = room1->getText();
  EXPECT_TRUE(textResp);
  EXPECT_EQ(textResp.value(), text);
}


TEST_F(Interaction, update) {
  for (int i = 0; i < 5; i++) {
    auto playerInfo = std::make_unique<PlayerInfo>("uuid" + std::to_string(i));
    EXPECT_CALL(*db, FindPlayerInfoByUuid("uuid" + std::to_string(i)))
        .WillOnce(Return(ByMove(std::move(playerInfo))));

  }

  EXPECT_CALL(*db, UpadatePlayerInfo_(_))
      .Times(5);

  room->setRoomStatus(std::make_shared<RoomWait>(rc));
  service.run();

}

TEST_F(Interaction, insert) {
  for (int i = 0; i < 5; i++) {
    auto playerInfo = std::unique_ptr<PlayerInfo>();
    EXPECT_CALL(*db, FindPlayerInfoByUuid("uuid" + std::to_string(i)))
        .WillOnce(Return(ByMove(std::move(playerInfo))));

  }

  EXPECT_CALL(*db, InsertPlayerInfo_(_))
      .Times(5);

  room->setRoomStatus(std::make_shared<RoomWait>(rc));
  service.run();

}
