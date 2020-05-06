#include "TextInfoImpl.hpp"
#include <gtest/gtest.h>

const std::string kTestDataBaseName("noTypeTestDataBase");

TEST(TextInfoImpl, GetRandomText) {
  DataBase::Instance();
  std::unique_ptr<DataBase::External::ITextInfoMapper> TextMapper
   = std::make_unique<DataBase::External::TextInfoMapper>(kTestDataBaseName);

  const std::string testText = "Test text";

  auto textData = TextMapper->GetRandomText();

  EXPECT_EQ(textData->charCount, testText.size());
  EXPECT_EQ(textData->text, testText);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
