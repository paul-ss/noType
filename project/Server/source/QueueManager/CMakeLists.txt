ADD_LIBRARY(QueueLib STATIC
        Queue.cpp
        Queue.hpp
        QueueManager.cpp
        QueueManager.hpp)

TARGET_LINK_LIBRARIES(QueueLib LoggerLib)

ADD_EXECUTABLE(queueTests
        ../mainTest.cpp
        Tests/QueueTests.cpp
        Tests/QueueManagerTests.cpp)


TARGET_LINK_LIBRARIES(queueTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        QueueLib
        CommandLib
        LoggerLib
        -lgmock)