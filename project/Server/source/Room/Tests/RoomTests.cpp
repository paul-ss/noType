//
// Created by paul_s on 27.04.2020.
//

#include "gtest/gtest.h"
#include "Room.hpp"
#include "RoomManager.hpp"
#include <thread>


void playersEQ(const Player &l, const Player &r) {
  ASSERT_EQ(l._clientUUID, r._clientUUID);
  ASSERT_EQ(l._name, r._name);
  ASSERT_EQ(l._textPosition, r._textPosition);
  ASSERT_EQ(l._lastTextPosition, r._lastTextPosition);
  ASSERT_EQ(l._playerID, r._playerID);
  ASSERT_EQ(l._state, r._state);
}


TEST(room, game_session) {
  RoomConfig rc(200, 5, 500, 700, 700, 0);
  boost::asio::io_service service;
  auto rm =  std::make_shared<RoomManager>();
  std::vector<std::thread> thr;
  std::string text = "C++11 Examples: Contains a limited set of the C++03 Boost.Asio"
                     "examples, updated to use only C++11 library and language facilities."
                     "These examples do not make direct use of Boost C++ libraries."
                     "C++17 Examples: Selected examples illustrating C++17 usage in conjunction with Technical Specifications.";
  auto room = std::make_shared<Room>(service, text, rm, rc);

  Player player0("uuid0", "name0" );
  ASSERT_TRUE(room->addPlayer(player0));

  thr.emplace_back([&]() {service.run();});

  for (int i = 1; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }


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
    ASSERT_EQ(players.at("uuid" + std::to_string(i))._textPosition, i * i * 5);
  }
}



TEST(room, WAIT_add_player_get_text) {
  RoomConfig rc(200, 5, 1000, 0, 0, 0);
  boost::asio::io_service service;
  auto rm =  std::make_shared<RoomManager>();
  std::vector<std::thread> thr;
  std::string text = "text";
  auto room = std::make_shared<Room>(service, text, rm, rc);


  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }

  thr.emplace_back([&]() {service.run();});

  Player player1("uuid1", "name1" );
  ASSERT_THROW(room->addPlayer(player1), RoomException);

  Player player2("", "name1" );
  ASSERT_THROW(room->addPlayer(player1), RoomException);

  auto recvt = room->getText();
  ASSERT_TRUE(recvt);
  ASSERT_EQ(text, recvt.value());


  for (int i = 3; i < 5; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }


  for (auto &t : thr) {
    t.join();
  }
}



TEST(room, WAIT_other_funcs) {
  RoomConfig rc(200, 5, 1000, 0, 0, 0);
  boost::asio::io_service service;
  auto rm = std::make_shared<RoomManager>();
  std::vector<std::thread> thr;
  std::string text = "text";
  auto room = std::make_shared<Room>(service, text, rm, rc);

  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }

  ASSERT_FALSE(room->validateWrittenText("text", ""));
  ASSERT_FALSE(room->validateWrittenText("text", "uuid0"));
  ASSERT_FALSE(room->validateWrittenText("", ""));

  auto players = room->getPlayers();
  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    playersEQ(players.at("uuid" + std::to_string(i)), player);
  }
}




TEST(room, PLAY_validate) {
  RoomConfig rc(200, 5, 300, 1000, 500, 0);
  boost::asio::io_service service;
  auto rm = std::make_shared<RoomManager>();
  std::vector<std::thread> thr;
  std::string text = "C++11 Examples: Contains a limited set of the C++03 Boost.Asio"
                     "examples, updated to use only C++11 library and language facilities."
                     "These examples do not make direct use of Boost C++ libraries."
                     "C++17 Examples: Selected examples illustrating C++17 usage in conjunction with Technical Specifications.";
  auto room = std::make_shared<Room>(service, text, rm, rc);

  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }

  thr.emplace_back([&]() {service.run();});

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  size_t pos1 = 0;
  while(auto res = room->validateWrittenText(text.substr(pos1, 20), "uuid0")) {
    pos1 += res.value();
  }

  size_t pos2 = 0;
  while(auto res = room->validateWrittenText(text.substr(pos2, 17), "uuid1")) {
    pos2 += res.value();
  }

  auto players = room->getPlayers();

  for (auto &t : thr) {
    t.join();
  }

  ASSERT_EQ(players.at("uuid0")._textPosition, 200);
  ASSERT_EQ(players.at("uuid0")._state, PLAYER_WIN);
  ASSERT_GT(players.at("uuid1")._textPosition, 200);
  ASSERT_EQ(players.at("uuid1")._state, PLAYER_FINISH);
  ASSERT_EQ(players.at("uuid2")._textPosition, 0);
  ASSERT_EQ(players.at("uuid2")._state, PLAYER_PLAY);

}

TEST(room, PLAY_other_func) {
  RoomConfig rc(200, 5, 300, 1000, 500, 0);
  boost::asio::io_service service;
  auto rm = std::make_shared<RoomManager>();
  std::vector<std::thread> thr;
  std::string text = "text";
  auto room = std::make_shared<Room>(service, text, rm, rc);

  for (int i = 0; i < 3; i++) {
    Player player("uuid" + std::to_string(i), "name" + std::to_string(i));
    ASSERT_TRUE(room->addPlayer(player));
  }

  thr.emplace_back([&]() { service.run(); });

  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  ASSERT_THROW(room->validateWrittenText("text", "123"), RoomException);

  ASSERT_FALSE(room->addPlayer(Player("uuid", "name")));
  ASSERT_FALSE(room->addPlayer(Player("uuid1", "name")));
  ASSERT_FALSE(room->addPlayer(Player("", "")));

  auto recvt = room->getText();
  ASSERT_TRUE(recvt);
  ASSERT_EQ(text, recvt.value());

  for (auto &t : thr) {
    t.join();
  }
}
