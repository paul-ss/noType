
#include <Parse.hpp>
using namespace rapidjson;


int parsableFromJSON::parseFromJSON(const std::string &data) {
    Document doc;
    doc.Parse(data.c_str());

    int res;
    assert(doc.HasMember("commandType"));
    assert(doc["commandType"].IsInt());
    res = doc["commandType"].GetInt();
    return res;
}

