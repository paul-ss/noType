#include "PlayerInfoImpl.hpp"
#include <gtest/gtest.h>

const std::string kUuid = "owerkgi42345gwweb";

const std::string kTestDataBaseName("noTypeTestDataBase");

TEST(PlayerInfoImpl, InsertAndFind) {
  std::unique_ptr<DataBase::External::IPlayerInfoMapper> playerInfoMapper
   = std::make_unique<DataBase::External::PlayerInfoMapper>(kTestDataBaseName);

  auto insertedPlayerData = std::make_unique<DataBase::External::PlayerInfo>(kUuid);

  playerInfoMapper->Insert(std::move(insertedPlayerData));

  auto extractedPlayerData = playerInfoMapper->FindByUuid(kUuid);

  EXPECT_EQ(extractedPlayerData->uuid, kUuid);
}

TEST(PlayerInfoImpl, Update) {
  std::unique_ptr<DataBase::External::IPlayerInfoMapper> playerInfoMapper
   = std::make_unique<DataBase::External::PlayerInfoMapper>(kTestDataBaseName);

  auto updatablePlayerData = playerInfoMapper->FindByUuid(kUuid);

  const std::int32_t kUpdatablePoints = 200;
  const std::int32_t kUpdatableWinsCount = 1;
  updatablePlayerData->points = kUpdatablePoints;
  updatablePlayerData->winsCount = kUpdatableWinsCount;

  playerInfoMapper->Update(std::move(updatablePlayerData));

  auto extractedPlayerData = playerInfoMapper->FindByUuid(kUuid);

  EXPECT_EQ(extractedPlayerData->uuid, kUuid);
  EXPECT_EQ(extractedPlayerData->points, kUpdatablePoints);
  EXPECT_EQ(extractedPlayerData->winsCount, kUpdatableWinsCount);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
