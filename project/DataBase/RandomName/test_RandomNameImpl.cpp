#include "RandomNameImpl.hpp"
#include <gtest/gtest.h>

const std::string kTestDataBaseName("noTypeTestDataBase");

TEST(RandomNameImpl, GetRandomName) {
  std::unique_ptr<DataBase::Internal::IRandomNameGenerator> nameGenerator
   = std::make_unique<DataBase::Internal::RandomNameGenerator>(kTestDataBaseName);

  const std::string name = "white Alligator";

  auto nameData = nameGenerator->GetRandomName();

  EXPECT_EQ(nameData->name, name);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
