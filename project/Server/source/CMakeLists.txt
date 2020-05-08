INCLUDE_DIRECTORIES(
        Controller
        QueueManager
        Queue
        Room
        RoomStatus
        TcpServer
        Command
        Utils
        Exceptions
        Setup
        DataBase/DataBaseFacade
        DataBase/DataBaseConfig
        DataBase/DataBaseInit
        DataBase/MongoCxxInit
        DataBase/PlayerInfo
        DataBase/RandomName
        DataBase/TextInfo)

message(find_path(Setup))


FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


FIND_PACKAGE(Threads REQUIRED)


SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

ADD_SUBDIRECTORY(DataBase/)
#MESSAGE(${PUBLIC})
INCLUDE_DIRECTORIES(PUBLIC ${DATA_BASE_FACADE_INCLUDE_DIRS})

ADD_LIBRARY(ServerLib STATIC
        TcpServer/Client.cpp
        TcpServer/Client.hpp
        TcpServer/ConnectedClients.cpp
        TcpServer/ConnectedClients.hpp
        TcpServer/TcpServer.cpp
        TcpServer/TcpServer.hpp
        TcpServer/ServerConfig.hpp)


ADD_LIBRARY(RoomLib STATIC
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


ADD_LIBRARY(QueueLib STATIC
        QueueManager/Queue.cpp
        QueueManager/Queue.hpp
        QueueManager/QueueManager.cpp
        QueueManager/QueueManager.hpp)



ADD_LIBRARY(CommandLib STATIC
        Command/Command.cpp
        Command/Command.hpp
        Command/CommandFactory.cpp
        Command/CommandFactory.hpp)

#ADD_LIBRARY(Controller STATIC
#        Controller/BasicController.cpp
#        Controller/BasicController.hpp
#        Controller/GameController.cpp
#        Controller/GameController.hpp)



ADD_EXECUTABLE(server
        main.cpp
        Controller/BasicController.cpp
        Controller/BasicController.hpp
        Controller/GameController.cpp
        Controller/GameController.hpp
        Exceptions/Exceptions.hpp
        Setup/Setup.cpp
        Setup/Setup.hpp)



TARGET_LINK_LIBRARIES(server
        ${Boost_SYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        DataBaseFacadeLib
        ServerLib
        RoomLib
        QueueLib
        CommandLib
#        Controller
        )

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
        RoomLib)


ADD_EXECUTABLE(tcpServerTests
        mainTest.cpp
        TcpServer/Tests/ClientTests.cpp
        TcpServer/Tests/ConnectedClientsTest.cpp)

TARGET_LINK_LIBRARIES(tcpServerTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        ServerLib
        QueueLib
        -lgmock)


ADD_EXECUTABLE(queueTests
        mainTest.cpp
        QueueManager/Tests/QueueTests.cpp
        QueueManager/Tests/QueueManagerTests.cpp)



TARGET_LINK_LIBRARIES(queueTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        QueueLib
        -lgmock)

#add_executable(tst)
#target_link_libraries (tst ${CMAKE_THREAD_LIBS_INIT}


#add_executable(tests ...)
#target_link_libraries(tests ${GTEST_LIBRARIES} ...)



#enable_testing()
#add_test(tests "./tests")

