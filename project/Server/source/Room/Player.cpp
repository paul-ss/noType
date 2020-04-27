//
// Created by paul_s on 20.04.2020.
//

#include "Player.hpp"


Player::Player(const std::string &clientUUID, const std::string &name) :
    _clientUUID(clientUUID),
    _playerID(0),
    _name(name),
    _textPosition(0),
    _state(PLAYER_PLAY),
    _lastTextPosition(0),
    _currentSpeed(0) {}