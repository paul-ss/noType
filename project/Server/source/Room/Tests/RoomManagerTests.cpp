//
// Created by paul_s on 28.04.2020.
//


#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"

TEST(room_manager, add_room_and_player) {
  RoomConfig rc(200, 5, 1000, 2000, 700, 0);
  boost::asio::io_service service;
  auto rm = std::make_shared<RoomManager>();
  
  ASSERT_FALSE(rm->addPlayer(Player("uuid", "name")));
  ASSERT_THROW(rm->addPlayer(Player("", "name")), RoomManagerException);

  ASSERT_THROW(rm->addPlayerAndRoom(Player("", "name"), service, "text", rc), RoomManagerException);
  ASSERT_THROW(rm->addPlayerAndRoom(Player("uuid", ""), service, "text", rc), RoomException);

  ASSERT_TRUE(rm->addPlayerAndRoom( Player("uuid0", "name"), service, "text", rc));
  ASSERT_THROW(rm->addPlayerAndRoom(Player("uuid0", ""), service, "text", rc), RoomManagerException);
  ASSERT_THROW(rm->addPlayer(Player("uuid0", "name")), RoomManagerException);

  for (int i = 1; i < 5; i++) {
    ASSERT_TRUE(rm->addPlayer(Player("uuid" + std::to_string(i), "name")));
  }
  ASSERT_FALSE(rm->addPlayer(Player("uuid5", "name")));
}



TEST(room_manager, delete_get_room) {
  RoomConfig rc(200, 5, 1000, 2000, 700, 0);
  boost::asio::io_service service;
  auto rm = std::make_shared<RoomManager>();

  for (int i = 0; i < 27; i++) {
    ASSERT_TRUE(rm->addPlayerAndRoom(Player("uuid" + std::to_string(i), "name"), service, "text", rc));
  }

  for (int i = 0; i < 27; i++) {
    ASSERT_TRUE(rm->getRoom("uuid" + std::to_string(i)));
  }
  ASSERT_FALSE(rm->getRoom("uu"));

  ASSERT_TRUE(rm->deleteRoom(rm->getRoom("uuid0")->getUUID()));

  for (int i = 0; i < 5; i++) {
    ASSERT_FALSE(rm->getRoom("uuid" + std::to_string(i)));
  }

  ASSERT_TRUE(rm->getRoom("uuid5"));
}