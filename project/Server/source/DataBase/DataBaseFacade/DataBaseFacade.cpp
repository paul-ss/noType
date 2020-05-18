#include "DataBaseFacade.hpp"

DataBaseFacade::DataBaseFacade() : DataBaseFacade(DataBase::kDataBaseName) {}

DataBaseFacade::DataBaseFacade(const std::string& dataBaseName)
 : _dataBaseName(dataBaseName), _playerInfoMapper(_dataBaseName), _textInfoMapper(_dataBaseName) {}

void DataBaseFacade::InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) {
  std::unique_lock<std::mutex> lock(_mtx);
  _playerInfoMapper.Insert(std::move(plyerInfo));
}

void DataBaseFacade::UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) {
  std::unique_lock<std::mutex> lock(_mtx);
  _playerInfoMapper.Update(std::move(plyerInfo));
}

std::unique_ptr<DataBase::External::PlayerInfo> DataBaseFacade::FindPlayerInfoByUuid(const std::string& uuid) {
  std::unique_lock<std::mutex> lock(_mtx);
  auto playerInfo = _playerInfoMapper.FindByUuid(uuid);
  return playerInfo;
}

std::unique_ptr<DataBase::External::TextInfo> DataBaseFacade::GetRandomText() {
  std::unique_lock<std::mutex> lock(_mtx);
  auto textInfo = _textInfoMapper.GetRandomText();
  return textInfo;
}
