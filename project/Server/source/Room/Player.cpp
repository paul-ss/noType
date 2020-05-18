//
// Created by paul_s on 20.04.2020.
//

#include "Player.hpp"


Player::Player(const std::string &clientUUID, const std::string &name) :
    clientUUID(clientUUID),
    playerID(randomUUID()),
    name(name),
    textPosition(0),
    state(PLAYER_PLAY),
    lastTextPosition(0),
    currentSpeed(0) {}


std::string Player::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}