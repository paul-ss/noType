//
// Created by paul_s on 20.04.2020.
//

#pragma once

#include <string>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include <Parse.hpp>


enum PlayerState {PLAYER_PLAY, PLAYER_FINISH, PLAYER_WIN};

struct Player {
public:
  Player(const std::string &clientUUID, const std::string &name);
  rapidjson::Document toJSON();

public:
  const std::string clientUUID;
  const std::string playerID;
  const std::string name;
  size_t textPosition;
  PlayerState state;

  size_t lastTextPosition;
  double currentSpeed;


private:
  std::string randomUUID();
};

