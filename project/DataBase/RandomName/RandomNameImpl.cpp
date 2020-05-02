#include "RandomNameImpl.hpp"

#include <random>

namespace DataBase {
namespace Internal {

static std::string get_random_document_by_id_and_field(
  const std::unique_ptr<mongocxx::collection>& collection, const char* idField, const char* FieldName);

RandomNameGenerator::RandomNameGenerator() {
  // NOTE(vendroid): Возможно неиспользуемая переменная.
  mongocxx::instance inst{};
  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[kDataBaseName];
  _colorsCollection = std::make_unique<mongocxx::collection>(db[kColorsCollectionName]);
  _animalNamesCollection = std::make_unique<mongocxx::collection>(db[kAnimalsCollectionName]);
}

std::unique_ptr<RandomName> RandomNameGenerator::GetRandomName() {
  std::string firstName = get_random_document_by_id_and_field(
    _colorsCollection, _kAnimalIdField, _kAnimalNameField);

  std::string secondName = get_random_document_by_id_and_field(
    _colorsCollection, _kColorIdField, _kColorNameField);

  return std::make_unique<RandomName>(firstName + secondName);
}

static std::string get_random_document_by_id_and_field(
  const std::unique_ptr<mongocxx::collection>& collection, const char* idField, const char* FieldName) {
  size_t docCount = collection->count_documents({});

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, docCount);

  const std::int32_t kRandId = dis(gen);
  auto maybe_result = collection->find_one(
        bsoncxx::builder::stream::document{}
        << std::string(idField) << kRandId
        <<  bsoncxx::builder::stream::finalize);

  if (!maybe_result) {
    // TODO(vendroid): Сделать обработку ошибки
  }

  auto docView = maybe_result->view();
  return docView[FieldName].get_utf8().value.to_string();
}

}  // Internal
}  // DataBase