#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "Logger.hpp"

static const std::string testArg = "test";

std::vector<bsoncxx::document::value> loadAnimalsNames(const std::string& root) {
    std::ifstream file(root + "animals.txt");

    int i = 0;
    std::vector<bsoncxx::document::value> documents;
    std::string animalName;
    while(std::getline(file, animalName)) {
        documents.push_back(
            bsoncxx::builder::stream::document{}
            << "AnimalId" << ++i
            << "AnimalName" << animalName
            << bsoncxx::builder::stream::finalize
        );
    }

    file.close();

    return documents;
}

std::vector<bsoncxx::document::value> loadColorNames(const std::string& root) {
    std::ifstream file(root + "colors.txt");

    int i = 0;
    std::vector<bsoncxx::document::value> documents;
    std::string colorName;
    while(std::getline(file, colorName)) {
        documents.push_back(
            bsoncxx::builder::stream::document{}
            << "ColorId" << ++i
            << "ColorName" << colorName
            << bsoncxx::builder::stream::finalize
        );
    }
    documents.shrink_to_fit();

    file.close();

    return documents;
}

std::vector<bsoncxx::document::value> loadTexts(const std::string& root) {
    std::ifstream file(root + "texts.txt");

    int i = 0;
    std::vector<bsoncxx::document::value> documents;
    std::string text;
    while(std::getline(file, text)) {
        documents.push_back(
            bsoncxx::builder::stream::document{}
            << "TextId" << ++i
            << "CharCount" << static_cast<int>(text.size())
            << "Text" << text
            << bsoncxx::builder::stream::finalize
        );
    }
    documents.shrink_to_fit();

    file.close();

    return documents;
}

void insertCollectionToDataBase(const mongocxx::database& db, const std::string& collectionName, std::vector<bsoncxx::document::value>& documents) {
    mongocxx::collection collection = db[collectionName];

    if (std::int64_t doc_count = collection.count_documents({}); doc_count != 0) {
        //std::cout << "Collection \"" << collectionName << "\" is up to date!" << std::endl;
        BOOST_LOG_TRIVIAL(info) << "Collection \"" << collectionName << "\" is up to date!";
        return;
    }

    collection.insert_many(documents);
}

int main(int argc, char *argv[]) {
    mongocxx::instance inst{};
    mongocxx::client client{mongocxx::uri{}};

    std::string dataBaseName;
    const std::string root("source/DataBase/DataBaseInit/");
    std::string dir;
    if (argc == 1) {
        dir = root + "Data/";
        dataBaseName = "noTypeDataBase";
    } else if (argc == 2 && (std::string(argv[1]) == testArg)) {
        dir = root + "TestData/";
        dataBaseName = "noTypeTestDataBase";
    } else {
        return EXIT_FAILURE;
    }

    mongocxx::database db = client[dataBaseName];

    auto animalDocuments = loadAnimalsNames(dir);
    insertCollectionToDataBase(db, "Animals", animalDocuments);

    auto colorsDocuments = loadColorNames(dir);
    insertCollectionToDataBase(db, "Colors", colorsDocuments);

    auto textsDosuments  = loadTexts(dir);
    insertCollectionToDataBase(db, "Texts", textsDosuments);

    return EXIT_SUCCESS;
}
