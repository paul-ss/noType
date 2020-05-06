#include "TextInfoImpl.hpp"

#include <random>

namespace DataBase {
namespace External {

TextInfoMapper::TextInfoMapper() : TextInfoMapper(kDataBaseName) {}

TextInfoMapper::TextInfoMapper(const std::string& dataBaseName) : _dataBaseName(dataBaseName) {
  Instance();
}

std::unique_ptr<TextInfo> TextInfoMapper::GetRandomText() {
  // NOTE(vendroid): Возможно неиспользуемая переменная.
  // mongocxx::instance inst{};

  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[_dataBaseName];
  auto collection = db[kTextInfoCollectionName];
  size_t docCount = collection.count_documents({});

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, docCount);

  const auto kRandTextId = dis(gen);
  auto maybe_result = collection.find_one(
        bsoncxx::builder::stream::document{}
        << _kTextIdField << kRandTextId <<
        bsoncxx::builder::stream::finalize);

  if (!maybe_result) {
    return nullptr;
  }

  auto docView = maybe_result->view();
  size_t charCount = docView[_kCharCountField].get_int32().value;
  std::string text = docView[_kTextField].get_utf8().value.to_string();

  return std::make_unique<TextInfo>(charCount, std::move(text));
}

}  // External
}  // DataBase
