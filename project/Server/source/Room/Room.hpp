//
// Created by paul_s on 16.04.2020.
//

#pragma once
#include <iostream>

typedef struct Player_struct Player;

enum playerStatus{PLAYER, WINNER, LOST};

struct Player_struct{
    unsigned int id;
    std::string name;
    unsigned int position;
    double speed;
    playerStatus statusOfPlayer;
};

class Room {

};


