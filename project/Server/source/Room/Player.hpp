//
// Created by paul_s on 20.04.2020.
//

#pragma once

#include <string>
#include <chrono>


enum PlayerState {PLAYER_PLAY, PLAYER_FINISH, PLAYER_WIN};

struct Player {

  Player(const std::string &clientUUID, const std::string &name);
  const std::string _clientUUID;
  const size_t _playerID;
  const std::string _name;
  size_t _textPosition;
  PlayerState _state;

  size_t _lastTextPosition;
  double _currentSpeed;
};


