#include "PlayerInfo.hpp"

#include <DataBaseConfig.hpp>

namespace DataBase {
namespace External {

class PlayerInfoMapper : public IPlayerInfoMapper {
public:
  PlayerInfoMapper();
  std::unique_ptr<PlayerInfo> FindByUuid(const std::string &uuid);
  void Insert(std::unique_ptr<PlayerInfo> playerInfo);
  void Update(std::unique_ptr<PlayerInfo> playerInfo);

private:
  std::unique_ptr<mongocxx::collection> _playerInfoCollection = nullptr;

private:
  std::unique_ptr<PlayerInfo> construct_player_info(const bsoncxx::document::view &docView);
  auto create_query_document(const std::unique_ptr<PlayerInfo>& playerInfo);

private:
  static constexpr char _kNameField[] = "Name";
  static constexpr char _kUuidField[] = "Uuid";
  static constexpr char _kWinsCountField[] = "WinsCount";
  static constexpr char _kPointsField[] = "Points";
};

}  // External
}  // DataBase
