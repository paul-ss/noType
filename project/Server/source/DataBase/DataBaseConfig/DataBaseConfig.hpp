#pragma once

#include "DataBaseException.hpp"

#include <mongocxx/client.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/exception/exception.hpp>

#include <bsoncxx/exception/exception.hpp>

namespace DataBase {

static constexpr char kDataBaseName[] = "noTypeDataBase";

namespace External {

static constexpr char kTextInfoCollectionName [] = "Texts";
static constexpr char kPlayerInfoCollectionName [] = "PlayersInfo";

}  // External

namespace Internal {

static constexpr char kColorsCollectionName[] = "Colors";
static constexpr char kAnimalsCollectionName[] = "Animals";

}  // Internal

}  // DataBase
