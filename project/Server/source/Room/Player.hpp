//
// Created by paul_s on 20.04.2020.
//

#pragma once

#include <string>

enum PlayerState {PLAYER_PLAY, PLAYER_FINISH, PLAYER_WIN};

struct Player {

  Player(const std::string &clientUUID, const std::string &name);

  std::string _clientUUID;
  std::string _name;
  size_t _textPosition;
  PlayerState _state;
  int i;
  double _currentSpeed;
};


