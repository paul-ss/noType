#include "DataBaseFacade.hpp"
#include <gtest/gtest.h>
#include <thread>

const std::string kUuid = "TestUuuid1";

const std::string kTestDataBaseName("noTypeTestDataBase");

TEST(DataBaseFacade, InsertAndFind) {
  // DataBaseFacade dataBaseFacade(kTestDataBaseName);

  // auto insertedPlayerData = std::make_unique<DataBase::External::PlayerInfo>(kUuid);

  // dataBaseFacade.InsertPlayerInfo(std::move(insertedPlayerData));

  // auto extractedPlayerData = dataBaseFacade.FindPlayerInfoByUuid(kUuid);

  // EXPECT_EQ(extractedPlayerData->uuid, kUuid);
}

TEST(DataBaseFacade, ThreadSafetyCheck) {
  DataBaseFacade dataBaseFacade(kTestDataBaseName);

  std::mutex mtx;

  {
    auto threadfunc = [](DataBaseFacade& _dataBaseFacade, std::mutex& _mtx, size_t shift) {
      for (size_t i = 0 + shift; i < 10 + shift; ++i) {
        auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
        _dataBaseFacade.InsertPlayerInfo(std::move(playerInfo));
        //threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
    }
    };

    std::thread t1([&]() {
    //   for (size_t i = 0; i < 10; ++i) {
    //     auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
    //     threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
    // }
      threadfunc(dataBaseFacade, mtx, 0);
    });

    std::thread t2([&]() {
    //   for (size_t i = 10; i < 20; ++i) {
    //     auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
    //     threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
    // }
    threadfunc(dataBaseFacade, mtx, 10);
    });

    t1.join();
    t2.join();
  }

  {
    auto threadfunc = [](DataBaseFacade& _dataBaseFacade, std::mutex& _mtx, size_t shift) {
      for (size_t i = 0 + shift; i < 10 + shift; ++i) {
        auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
        playerInfo->points = i;
        _dataBaseFacade.UpadatePlayerInfo(std::move(playerInfo));
        //threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
    }
    };

    std::thread t1([&]() {
    //   for (size_t i = 0; i < 10; ++i) {
    //     auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
    //     threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
    // }
      threadfunc(dataBaseFacade, mtx, 0);
    });

    std::thread t2([&]() {
    //   for (size_t i = 10; i < 20; ++i) {
    //     auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
    //     threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
    // }
    threadfunc(dataBaseFacade, mtx, 10);
    });

    t1.join();
    t2.join();
  }

  // {
  //   auto threadfunc = [](DataBaseFacade& _dataBaseFacade, std::unique_ptr<DataBase::External::PlayerInfo> _playerInfo, std::mutex& _mtx) {
  //     //_mtx.lock();
  //     _dataBaseFacade.UpadatePlayerInfo(std::move(_playerInfo));
  //     //_mtx.unlock();
  //   };

  //   std::thread t1([&]() {
  //     for (size_t i = 0; i < 10; ++i) {
  //       auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
  //       playerInfo->points = i;
  //       threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
  //     }
  //   });

  //   std::thread t2([&]() {
  //     for (size_t i = 10; i < 20; ++i) {
  //       auto playerInfo = std::make_unique<DataBase::External::PlayerInfo>(std::to_string(i));
  //       playerInfo->points = i;
  //       threadfunc(dataBaseFacade, std::move(playerInfo), mtx);
  //   }});

  //   t1.join();
  //   t2.join();
  // }



  // EXPECT_EQ(extractedPlayerData->uuid, kUuid);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}