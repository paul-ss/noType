//
// Created by paul_s on 20.04.2020.
//

#include "Player.hpp"


Player::Player(const std::string &clientUUID, const std::string &name) :
    clientUUID(clientUUID),
    playerID(0),
    name(name),
    textPosition(0),
    state(PLAYER_PLAY),
    lastTextPosition(0),
    currentSpeed(0) {}