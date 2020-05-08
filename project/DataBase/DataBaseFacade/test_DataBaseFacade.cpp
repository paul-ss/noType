#include "DataBaseFacade.hpp"
#include <gtest/gtest.h>

const std::string kUuid = "owerkgi42345gwweb";

const std::string kTestDataBaseName("noTypeTestDataBase");

TEST(DataBaseFacade, InsertAndFind) {
  DataBaseFacade dataBaseFacade(kTestDataBaseName);

  auto insertedPlayerData = std::make_unique<DataBase::External::PlayerInfo>(kUuid);

  dataBaseFacade.InsertPlayerInfo(std::move(insertedPlayerData));

  auto extractedPlayerData = dataBaseFacade.FindPlayerInfoByUuid(kUuid);

  EXPECT_EQ(extractedPlayerData->uuid, kUuid);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}