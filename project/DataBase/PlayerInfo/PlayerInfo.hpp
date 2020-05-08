#pragma once

#include <string>

namespace DataBase {
namespace External {

struct PlayerInfo {
  PlayerInfo(const std::string& uuid, const std::string& name = {}, std::int32_t winsCount = 0, std::int32_t points = 0)
   : uuid(uuid), name(name), winsCount(winsCount), points(points) {}
  std::string uuid;
  std::string name;
  std::int32_t winsCount;
  std::int32_t points;
};

class IPlayerInfoMapper {
public:
  virtual std::unique_ptr<PlayerInfo> FindByUuid(const std::string &uuid) = 0;
  virtual void Insert(std::unique_ptr<PlayerInfo> playerInfo) = 0;
  virtual void Update(std::unique_ptr<PlayerInfo> playerInfo) = 0;
  virtual ~IPlayerInfoMapper() = default;
};

}  // External
}  // DataBase