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
        Parser
        Logger)


FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


FIND_PACKAGE(Threads REQUIRED)


SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREAD ON)
##
set(Boost_USE_STATIC_RUNTIME    OFF)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS
        system
        thread
        regex
        log
        log_setup
        program_options
)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


ADD_LIBRARY(LoggerLib STATIC
        Logger/Logger.hpp
        Logger/Logger.cpp)

TARGET_INCLUDE_DIRECTORIES(LoggerLib PUBLIC Logger/ ${Boost_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(LoggerLib PUBLIC ${Boost_LIBRARIES})

message(boost = ${Boost_LIBRARIES})

ADD_SUBDIRECTORY(DataBase/)
ADD_SUBDIRECTORY(TcpServer/)
ADD_SUBDIRECTORY(Room/)
ADD_SUBDIRECTORY(QueueManager/)


ADD_LIBRARY(RoomLib STATIC
        Room/RoomManager.cpp
        Room/RoomManager.hpp
        Room/Player.hpp
        Room/Player.cpp
        Room/Room.hpp
        Room/Room.cpp
        Room/RoomConfig.hpp
        RoomStatus/IRoomStatus.hpp
        RoomStatus/RoomWait.hpp
        RoomStatus/RoomPlay.hpp
        RoomStatus/RoomEnd.hpp
        RoomStatus/RoomWait.cpp
        RoomStatus/RoomPlay.cpp
        RoomStatus/RoomEnd.cpp
        Utils/Expected.hpp
        Utils/Error.hpp)


TARGET_LINK_LIBRARIES(RoomLib DataBaseFacadeLib LoggerLib)


ADD_LIBRARY(CommandLib STATIC
        Parser/Parse.hpp
        Parser/Parse.cpp
        Command/Command.cpp
        Command/Command.hpp
        Command/CommandFactory.cpp
        Command/CommandFactory.hpp
        Command/StartGameSession.cpp
        Command/StartGameSession.hpp
        Room/Player.hpp
        Room/Player.cpp
        RoomStatus/IRoomStatus.hpp
        Command/GetText.cpp
        Command/GetText.hpp
        Command/GetRoomStatus.cpp
        Command/GetRoomStatus.hpp
        Command/ValidateWrittenText.cpp
        Command/ValidateWrittenText.hpp
        Command/Init.cpp
        Command/Init.hpp
        Command/Connect.cpp
        Command/Connect.hpp
        Utils/Error.hpp)


TARGET_LINK_LIBRARIES(CommandLib LoggerLib ${Boost_LIBRARIES})



#ADD_EXECUTABLE(commandTests
#        Command/Tests/ParseFromTests.cpp
#        mainTest.cpp)
#
#TARGET_LINK_LIBRARIES(commandTests
#        ${GTEST_LIBRARIES}
#        ${CMAKE_THREAD_LIBS_INIT}
#        ${Boost_SYSTEM_LIBRARY}
#        CommandLib)



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
        ${Boost_LIBRARIES}
        DataBaseFacadeLib
        ServerLib
        RoomLib
        QueueLib
        CommandLib
        LoggerLib)










#enable_testing()
#add_test(tests "./tests")

## TODO (PavelS) : make Gtests
#ADD_EXECUTABLE(parsingfrom Parser/Test/testparsefrom.cpp)
#
#TARGET_LINK_LIBRARIES(parsingfrom
#        ${Boost_SYSTEM_LIBRARY}
#        ${CMAKE_THREAD_LIBS_INIT}
#        CommandLib)
#
#ADD_EXECUTABLE(parsingto Parser/Test/testparseto.cpp)
#
#TARGET_LINK_LIBRARIES(parsingto
#        ${Boost_SYSTEM_LIBRARY}
#        ${CMAKE_THREAD_LIBS_INIT}
#        CommandLib)