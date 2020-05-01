#include "TextInfoImpl.hpp"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <random>

namespace DataBase {
namespace External {

TextInfoMapper::TextInfoMapper() {
  // NOTE(vendroid): Возможно неиспользуемая переменная.
  mongocxx::instance inst{};
  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[kDataBaseName];
  _collection = std::make_unique<mongocxx::collection>(db[kTextInfoCollectionName]);
}

std::unique_ptr<TextInfo> TextInfoMapper::GetRandomText() {
  size_t docCount = _collection->count_documents({});

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, docCount);

  const std::uint32_t kRandTextId = dis(gen);
  auto maybe_result = _collection->find_one(
        bsoncxx::builder::stream::document{} << _kTextIdField << kRandTextId <<  bsoncxx::builder::stream::finalize);

  if (!maybe_result) {
    // TODO(vendroid): Сделать обработку ошибки
  }

  auto docView = maybe_result->view();
  size_t charCount = docView[_kCharCountField].get_int32().value;
  std::string text = docView[_kTextField].get_utf8().value.to_string();

  return std::make_unique<TextInfo>(charCount, std::move(text));
}

}  // External
}  // DataBase
