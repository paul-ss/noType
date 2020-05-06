INCLUDE_DIRECTORIES(
        Controller
        QueueManager
        Room
        TcpServer
        Parser
        Command)


FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


FIND_PACKAGE(Threads REQUIRED)


SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})





ADD_EXECUTABLE(server
        main.cpp
        TcpServer/TcpServer.cpp
        TcpServer/TcpServer.hpp
        TcpServer/Client.cpp
        TcpServer/Client.hpp
        Controller/BasicController.cpp
        Controller/BasicController.hpp
        Controller/GameController.cpp
        Controller/GameController.hpp
        Room/Room.cpp
        Room/Room.hpp
        QueueManager/QueueManager.cpp
        QueueManager/QueueManager.hpp
        Parser/Parse.cpp
        Command/Command.cpp
        Command/Command.hpp Command/CommandFactory.cpp Command/CommandFactory.hpp Parser/Parse.hpp Room/Player.hpp Room/Player.cpp)


TARGET_LINK_LIBRARIES(server ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
#SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

ADD_EXECUTABLE(serverTests
        mainTest.cpp)

TARGET_LINK_LIBRARIES(serverTests ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})



#add_executable(tst)
#target_link_libraries (tst ${CMAKE_THREAD_LIBS_INIT}


#add_executable(tests ...)
#target_link_libraries(tests ${GTEST_LIBRARIES} ...)



#enable_testing()
#add_test(tests "./tests")

#ADD_EXECUTABLE(parsing testmain.cpp
 #       Parser/Parse.cpp
  #      Parser/Parse.hpp
   #     Command/Command.cpp
    #    Command/Command.hpp
     #   Command/CommandFactory.cpp
      #  Command/CommandFactory.hpp
       # )
ADD_EXECUTABLE(parsing testmain.cpp
        Parser/Parse.cpp
        Parser/Parse.hpp
        Command/Command.hpp
        Command/Command.cpp
        Command/CommandFactory.hpp
        Command/CommandFactory.cpp
        )