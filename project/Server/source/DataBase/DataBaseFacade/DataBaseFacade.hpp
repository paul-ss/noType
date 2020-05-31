#pragma once

#include "DataBaseFacadeInterface.hpp"
#include <thread>
#include <mutex>

class DataBaseFacade : public IDataBaseFacade {
public:
  DataBaseFacade();
  DataBaseFacade(const std::string& dataBaseName);

// NOTE(vendroid): Работа с таблицей информации о игроках
  void InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo);
  void UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo);

  std::unique_ptr<DataBase::External::PlayerInfo> FindPlayerInfoByUuid(const std::string& uuid);

// NOTE(vendroid): Работа с таблицей содержащих тексты
  std::unique_ptr<DataBase::External::TextInfo> GetRandomText();

private:
  std::mutex _mtx;
  std::string _dataBaseName;
  DataBase::External::PlayerInfoMapper _playerInfoMapper;
  DataBase::External::TextInfoMapper _textInfoMapper;
};
