#include "PlayerInfoImpl.hpp"
#include "RandomNameImpl.hpp"

namespace DataBase {
namespace External {

PlayerInfoMapper::PlayerInfoMapper() : PlayerInfoMapper(kDataBaseName) {}

PlayerInfoMapper::PlayerInfoMapper(const std::string& dataBaseName) : _dataBaseName(dataBaseName) {
  Instance();
}

std::unique_ptr<PlayerInfo> PlayerInfoMapper::FindByUuid(const std::string &uuid) {
  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[kDataBaseName];
  mongocxx::collection playerInfoCollection = db[kPlayerInfoCollectionName];

  auto maybe_result =  playerInfoCollection.find_one(
    bsoncxx::builder::stream::document{}
    << _kUuidField << uuid
    <<  bsoncxx::builder::stream::finalize);

  if (!maybe_result) {
    return nullptr;
  }

  auto docView = maybe_result->view();
  return construct_player_info(docView);
}

void PlayerInfoMapper::Insert(std::unique_ptr<PlayerInfo> playerInfo) {
  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[kDataBaseName];
  mongocxx::collection playerInfoCollection = db[kPlayerInfoCollectionName];

  Internal::RandomNameGenerator randomNameGenerator(_dataBaseName);
  playerInfo->name = std::move(randomNameGenerator.GetRandomName()->name);

  auto maybe_result = playerInfoCollection.insert_one(create_query_document(playerInfo));
}

void PlayerInfoMapper::Update(std::unique_ptr<PlayerInfo> playerInfo) {
  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[kDataBaseName];
  mongocxx::collection playerInfoCollection = db[kPlayerInfoCollectionName];

  auto maybe_result = playerInfoCollection.find_one_and_update(
  bsoncxx::builder::stream::document{}
  << _kUuidField << playerInfo->uuid
  <<  bsoncxx::builder::stream::finalize,
  create_query_document(playerInfo));
}

std::unique_ptr<PlayerInfo> PlayerInfoMapper::construct_player_info(const bsoncxx::document::view &docView) {
  std::string name = docView[_kNameField].get_utf8().value.to_string();
  std::string uuid = docView[_kUuidField].get_utf8().value.to_string();
  size_t winsCount = docView[_kWinsCountField].get_int32();
  size_t points    = docView[_kPointsField].get_int32();

  return std::make_unique<PlayerInfo>(name, uuid, winsCount, points);
}

bsoncxx::document::value PlayerInfoMapper::create_query_document(const std::unique_ptr<PlayerInfo>& playerInfo) {
  return bsoncxx::builder::stream::document{}
  << _kUuidField      << playerInfo->uuid
  << _kNameField      << playerInfo->name
  << _kWinsCountField << static_cast<std::int64_t>(playerInfo->winsCount)
  << _kPointsField    << static_cast<std::int64_t>(playerInfo->points)
  <<  bsoncxx::builder::stream::finalize;
}

}  // External
}  // DataBase
