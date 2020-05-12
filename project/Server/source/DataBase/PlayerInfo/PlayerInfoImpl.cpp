#include "PlayerInfoImpl.hpp"
#include "RandomNameImpl.hpp"
#include "MongoCxxInit.hpp"

#include <iostream>

namespace DataBase {
namespace External {

PlayerInfoMapper::PlayerInfoMapper() : PlayerInfoMapper(kDataBaseName) {}

PlayerInfoMapper::PlayerInfoMapper(const std::string& dataBaseName) : _dataBaseName(dataBaseName) {
  try {
    MongoCxxInit::Instance();
  } catch (const mongocxx::exception& e) {
    throw Exception(e.what());
  }
}

std::unique_ptr<PlayerInfo> PlayerInfoMapper::FindByUuid(const std::string &uuid) {
  try {
    mongocxx::client client{mongocxx::uri{}};

    mongocxx::database db = client[_dataBaseName];
    mongocxx::collection playerInfoCollection = db[kPlayerInfoCollectionName];

    auto maybe_result = playerInfoCollection.find_one(
      bsoncxx::builder::stream::document{}
        << _kUuidField << uuid
        << bsoncxx::builder::stream::finalize);

    if (!maybe_result) {
      return nullptr;
    }

    auto docView = maybe_result->view();
    return construct_player_info(docView);
  } catch (const mongocxx::exception& e) {
    throw Exception(e.what());
  }
}

void PlayerInfoMapper::Insert(std::unique_ptr<PlayerInfo> playerInfo) {
  try {
    mongocxx::client client{mongocxx::uri{}};

    mongocxx::database db = client[_dataBaseName];
    mongocxx::collection playerInfoCollection = db[kPlayerInfoCollectionName];

    Internal::RandomNameGenerator randomNameGenerator(_dataBaseName);
    playerInfo->name = std::move(randomNameGenerator.GetRandomName()->name);

    auto maybe_result = playerInfoCollection.insert_one(create_query_document(playerInfo));
  } catch (const mongocxx::exception& e) {
    throw Exception(e.what());
  }
}

void PlayerInfoMapper::Update(std::unique_ptr<PlayerInfo> playerInfo) {
  try {
    mongocxx::client client{mongocxx::uri{}};

    mongocxx::database db = client[_dataBaseName];
    mongocxx::collection playerInfoCollection = db[kPlayerInfoCollectionName];

    auto maybe_result = playerInfoCollection.find_one_and_update(
    bsoncxx::builder::stream::document{}
    << _kUuidField << playerInfo->uuid
    <<  bsoncxx::builder::stream::finalize,
    create_query_document(playerInfo));
  } catch (const mongocxx::exception& e) {
    throw Exception(e.what());
  }
}

std::unique_ptr<PlayerInfo> PlayerInfoMapper::construct_player_info(const bsoncxx::document::view &docView) {
  try {
    std::string  uuid      = docView[_kUuidField].get_utf8().value.to_string();
    std::string  name      = docView[_kNameField].get_utf8().value.to_string();
    std::int32_t winsCount = docView[_kWinsCountField].get_int32().value;
    std::int32_t points    = docView[_kPointsField].get_int32().value;

    return std::make_unique<PlayerInfo>(uuid, name, winsCount, points);
  } catch (const mongocxx::exception& e) {
    throw Exception(e.what());
  }
}

bsoncxx::document::value PlayerInfoMapper::create_query_document(const std::unique_ptr<PlayerInfo>& playerInfo) {
  try {
    return bsoncxx::builder::stream::document{}
      << _kUuidField << playerInfo->uuid
      << _kNameField << playerInfo->name
      << _kWinsCountField << playerInfo->winsCount
      << _kPointsField << playerInfo->points
      << bsoncxx::builder::stream::finalize;
  } catch (const bsoncxx::exception& e) {
      throw Exception(e.what());
  }
}

}  // External
}  // DataBase
