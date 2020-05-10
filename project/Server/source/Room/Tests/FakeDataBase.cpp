//
// Created by paul_s on 09.05.2020.
//

#include "FakeDataBase.hpp"

using DataBase::External::TextInfo;

FakeDataBase::FakeDataBase(const std::string &text) :
    text(text) {}

void FakeDataBase::InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> playerInfo) {
  playerInfo->name.size();
}

void FakeDataBase::UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> playerInfo) {
  playerInfo->name.size();
}

std::unique_ptr<DataBase::External::PlayerInfo> FakeDataBase::FindPlayerInfoByUuid(const std::string& uuid) {
  uuid.size();
}

std::unique_ptr<DataBase::External::TextInfo> FakeDataBase::GetRandomText() {
  return std::make_unique<TextInfo>(0, std::string(text));
}

