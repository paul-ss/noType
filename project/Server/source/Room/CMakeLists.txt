ADD_EXECUTABLE(roomTests
        ../mainTest.cpp
        Tests/RoomTests.cpp
        Tests/RoomManagerTests.cpp
        Tests/RoomWaitTest.cpp
        Tests/RoomPlayTest.cpp
        Tests/FakeDataBase.hpp
        Tests/FakeDataBase.cpp
        Tests/InteractionTest.cpp)


TARGET_LINK_LIBRARIES(roomTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        LoggerLib
        RoomLib
        -lgmock)