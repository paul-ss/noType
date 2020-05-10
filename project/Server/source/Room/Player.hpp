//
// Created by paul_s on 20.04.2020.
//

#pragma once

#include <string>
#include <Parse.hpp>

enum PlayerState {PLAYER_PLAY, PLAYER_FINISH, PLAYER_WIN};

struct Player {

    Player(const std::string &clientUUID, const std::string &name);
    rapidjson::Document toJSON();
    const std::string clientUUID;
    const size_t playerID;
    const std::string name;
    size_t textPosition;
    PlayerState state;

    size_t lastTextPosition;
    double currentSpeed;
};
