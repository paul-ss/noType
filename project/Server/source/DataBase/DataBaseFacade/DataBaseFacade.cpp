#include "DataBaseFacade.hpp"

DataBaseFacade::DataBaseFacade() : DataBaseFacade(DataBase::kDataBaseName) {}

DataBaseFacade::DataBaseFacade(const std::string& dataBaseName)
 : _dataBaseName(dataBaseName), _playerInfoMapper(_dataBaseName), _textInfoMapper(_dataBaseName) {}

void DataBaseFacade::InsertPlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) {
  _mtx.lock();
  _playerInfoMapper.Insert(std::move(plyerInfo));
  _mtx.unlock();
}

void DataBaseFacade::UpadatePlayerInfo(std::unique_ptr<DataBase::External::PlayerInfo> plyerInfo) {
  _mtx.lock();
  _playerInfoMapper.Update(std::move(plyerInfo));
  _mtx.unlock();
}

std::unique_ptr<DataBase::External::PlayerInfo> DataBaseFacade::FindPlayerInfoByUuid(const std::string& uuid) {
  _mtx.lock();
  auto playerInfo = _playerInfoMapper.FindByUuid(uuid);
  _mtx.unlock();
  return playerInfo;
}

std::unique_ptr<DataBase::External::TextInfo> DataBaseFacade::GetRandomText() {
  _mtx.lock();
  auto textInfo = _textInfoMapper.GetRandomText();
  _mtx.unlock();
  return textInfo;
}
