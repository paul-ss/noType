#pragma once

#include "TextInfoImpl.hpp"
#include "PlayerInfoImpl.hpp"

#include <memory>

class IDataBaseFacade {
public:
// NOTE(vendroid): Работа с таблицей информации о игроках
  virtual void InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo);
  virtual void UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo);

  virtual std::unique_ptr<DataBase::External::PlayerInfo> FindPlayerInfoByUuid(const std::string& uuid);
  virtual ~IDataBaseFacade() = default;

// NOTE(vendroid): Работа с таблицей содержащих тексты
  std::unique_ptr<DataBase::External::TextInfo> GetRandomText();
};
