ADD_LIBRARY(ServerLib STATIC
        Client.cpp
        Client.hpp
        ConnectedClients.cpp
        ConnectedClients.hpp
        TcpServer.cpp
        TcpServer.hpp
        ServerConfig.hpp)

TARGET_LINK_LIBRARIES(ServerLib LoggerLib)


ADD_EXECUTABLE(tcpServerTests
        ../mainTest.cpp
        Tests/ClientTests.cpp
        Tests/ConnectedClientsTest.cpp)

TARGET_LINK_LIBRARIES(tcpServerTests
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_SYSTEM_LIBRARY}
        ServerLib
        QueueLib
        CommandLib
        LoggerLib
        -lgmock)