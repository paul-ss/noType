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
        Parser)


FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


FIND_PACKAGE(Threads REQUIRED)


SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


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

TARGET_LINK_LIBRARIES(RoomLib DataBaseFacadeLib)



ADD_LIBRARY(CommandLib STATIC
        Parser/Parse.hpp
        Parser/Parse.cpp
        Command/Command.cpp
        Command/Command.hpp
        Command/CommandFactory.cpp
        Command/CommandFactory.hpp
        Room/Player.hpp
        Room/Player.cpp
        RoomStatus/IRoomStatus.hpp)




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
        CommandLib)










#enable_testing()
#add_test(tests "./tests")

# TODO (PavelS) : make Gtests
ADD_EXECUTABLE(parsingfrom Parser/Test/testparsefrom.cpp
        Parser/Parse.cpp
        Parser/Parse.hpp
        Command/Command.hpp
        Command/Command.cpp
        Command/CommandFactory.hpp
        Command/CommandFactory.cpp
        )

ADD_EXECUTABLE(parsingto Parser/Test/testparseto.cpp
        Parser/Parse.cpp
        Parser/Parse.hpp
        Command/Command.hpp
        Command/Command.cpp
        Command/CommandFactory.hpp
        Command/CommandFactory.cpp
        )