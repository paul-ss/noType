#include "RandomNameImpl.hpp"

#include <random>
#include <iostream>

namespace DataBase {
namespace Internal {

RandomNameGenerator::RandomNameGenerator() : RandomNameGenerator(kDataBaseName) {}

RandomNameGenerator::RandomNameGenerator(const std::string& dataBaseName) : _dataBaseName(dataBaseName) {
  Instance();
}

std::unique_ptr<RandomName> RandomNameGenerator::GetRandomName() {
  std::string firstName = get_random_document_by_id_and_field(
    kColorsCollectionName, _kColorIdField, _kColorNameField);

  std::string secondName = get_random_document_by_id_and_field(
    kAnimalsCollectionName, _kAnimalIdField, _kAnimalNameField);

  return std::make_unique<RandomName>(firstName + " " +  secondName);
}

std::string RandomNameGenerator::get_random_document_by_id_and_field(
  const char* collectionName, const char* idField, const char* fieldName) const {
  mongocxx::client client{mongocxx::uri{}};

  mongocxx::database db = client[_dataBaseName];
  auto collection = db[collectionName];

  size_t docsCount = collection.count_documents({});

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, docsCount);
  const auto kRandId = dis(gen);
  auto maybe_result = collection.find_one(
        bsoncxx::builder::stream::document{}
        << std::string(idField) << kRandId
        << bsoncxx::builder::stream::finalize);
  
  if (!maybe_result) {
    // TODO(vendroid): Сделать обработку ошибки
  }

  auto docView = maybe_result->view();
  return docView[fieldName].get_utf8().value.to_string();
}

}  // Internal
}  // DataBase
