INCLUDE_DIRECTORIES(
        Controller
        QueueManager
        Queue
        Room
        RoomStatus
        TcpServer
        Command
        Utils
        Exceptions)


FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


FIND_PACKAGE(Threads REQUIRED)


SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


ADD_LIBRARY(Server SHARED
        TcpServer/Client.cpp
        TcpServer/Client.hpp
        TcpServer/ConnectedClients.cpp
        TcpServer/ConnectedClients.hpp
        TcpServer/TcpServer.cpp
        TcpServer/TcpServer.hpp
        TcpServer/ServerConfig.hpp)

ADD_LIBRARY(Room SHARED
        Room/RoomManager.cpp
        Room/RoomManager.hpp
        Room/Player.hpp
        Room/Player.cpp
        Room/Room.hpp
        Room/Room.cpp
        RoomStatus/RoomWait.cpp
        RoomStatus/RoomWait.hpp
        RoomStatus/RoomPlay.cpp
        RoomStatus/RoomPlay.hpp
        RoomStatus/RoomEnd.cpp
        RoomStatus/RoomEnd.hpp
        Room/RoomConfig.hpp
        Utils/Expected.hpp
        Utils/Error.hpp)


ADD_LIBRARY(Queue SHARED
        QueueManager/Queue.cpp
        QueueManager/Queue.hpp
        QueueManager/QueueManager.cpp
        QueueManager/QueueManager.hpp)


ADD_LIBRARY(Command SHARED
        Command/Command.cpp
        Command/Command.hpp
        Command/CommandFactory.cpp
        Command/CommandFactory.hpp)

ADD_LIBRARY(Controller SHARED
        Controller/BasicController.cpp
        Controller/BasicController.hpp
        Controller/GameController.cpp
        Controller/GameController.hpp)



ADD_EXECUTABLE(server
        main.cpp
        Exceptions/Exceptions.hpp)



TARGET_LINK_LIBRARIES(server
        ${Boost_SYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        Server
        Room
        Queue
        Command
        Controller)

#SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

ADD_EXECUTABLE(roomTests
        mainTest.cpp
        Room/Tests/RoomTests.cpp
        Room/Tests/RoomManagerTests.cpp
        Room/Tests/RoomWaitTest.cpp
        Room/Tests/RoomPlayTest.cpp)

TARGET_LINK_LIBRARIES(roomTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        Room)


ADD_EXECUTABLE(tcpServerTests
        mainTest.cpp
        TcpServer/Tests/ClientTests.cpp
        TcpServer/Tests/ConnectedClientsTest.cpp)

TARGET_LINK_LIBRARIES(tcpServerTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        Server
        Queue
        -lgmock)


ADD_EXECUTABLE(queueTests
        mainTest.cpp
        QueueManager/Tests/QueueTests.cpp)

TARGET_LINK_LIBRARIES(queueTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        Queue
        -lgmock)


#add_executable(tst)
#target_link_libraries (tst ${CMAKE_THREAD_LIBS_INIT}


#add_executable(tests ...)
#target_link_libraries(tests ${GTEST_LIBRARIES} ...)



#enable_testing()
#add_test(tests "./tests")

