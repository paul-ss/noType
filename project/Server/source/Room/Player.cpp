
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

rapidjson::Document Player::toJSON(){
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();
    rapidjson::Value jsonVal;
    jsonVal.SetString(clientUUID.c_str(), allocator);
    jsonVal.AddMember("clientUUID", jsonVal, allocator);
    jsonVal.SetString(name.c_str(), allocator);
    jsonVal.AddMember("name", jsonVal, allocator);
    jsonVal.SetUint64(textPosition);
    jsonVal.AddMember("textPosition", jsonVal, allocator);
    return doc;
}