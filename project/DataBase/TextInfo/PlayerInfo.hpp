#pragma once

#include <string>

namespace DataBase {
namespace External {

struct PlayerInfo {
  std::string name;
  std::string uuid;
  size_t winsCount;
  size_t points;
};

class IPlayerInfoMapper {
public:
  virtual std::unique_ptr<PlayerInfo> FindByUuid(const std::string &uuid) = 0;
  virtual void Insert(std::unique_ptr<PlayerInfo> playerInfo) = 0;
  virtual void Update(std::unique_ptr<PlayerInfo> playerInfo) = 0;
};

}  // External
}  // DataBase