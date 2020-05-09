//
// Created by paul_s on 09.05.2020.
//

#pragma once

#include "DataBaseFacade.hpp"

class FakeDataBase : public IDataBaseFacade {
public:
  FakeDataBase() = default;
  FakeDataBase(const std::string &text);
  void InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) override;
  void UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) override;
  std::unique_ptr<DataBase::External::PlayerInfo> FindPlayerInfoByUuid(const std::string& uuid) override;
  std::unique_ptr<DataBase::External::TextInfo> GetRandomText() override;
private:
  std::string text;
};
