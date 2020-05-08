#include "DataBaseFacade.hpp"

DataBaseFacade::DataBaseFacade() : DataBaseFacade(DataBase::kDataBaseName) {}

DataBaseFacade::DataBaseFacade(const std::string& dataBaseName)
 : _dataBaseName(dataBaseName), _playerInfoMapper(_dataBaseName), _textInfoMapper(_dataBaseName) {}

void DataBaseFacade::InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) {
  _playerInfoMapper.Insert(std::move(plyerInfo));
}

void DataBaseFacade::UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) {
  _playerInfoMapper.Update(std::move(plyerInfo));
}

std::unique_ptr<DataBase::External::PlayerInfo> DataBaseFacade::FindPlayerInfoByUuid(const std::string& uuid) {
  return _playerInfoMapper.FindByUuid(uuid);
}

std::unique_ptr<DataBase::External::TextInfo> DataBaseFacade::GetRandomText() {
  return _textInfoMapper.GetRandomText();
}
