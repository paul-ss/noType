cmake_minimum_required(VERSION 3.10)
project(NetworkManager)

set(NETWORK_MANAGER_INCLUDE_DIRS
                                ../Message/
                                ../PlayerInfo/
                                ../MessageParser/
                                ../QueueManager/
                                ../../exceptions/
#                                ../../logger/
                                ../{$Boost_INCLUDE_DIR}
                                ${CMAKE_INCLUDE_CURRENT_DIR})

set(NETWORK_MANAGER_HEADERS
                                ../Message/Message.hpp
                                ../PlayerInfo/PlayerInfo.hpp
                                ../MessageParser/MessageParser.hpp
                                ../MessageParser/MessageParserConfig.hpp
                                ../QueueManager/QueueManager.hpp
                                ../../exceptions/exceptions.hpp
#                                ../../logger/logger.hpp
                                NetworkManager.hpp
                                NetworkManagerConfig.hpp)

set(NETWORK_MANAGER_SRCS
                                ../Message/Message.cpp
                                ../MessageParser/MessageParser.cpp
                                ../QueueManager/QueueManager.cpp
#                                ../../logger/logger.cpp
                                NetworkManager.cpp)


add_library(NetworkManagerLib STATIC
                                ${NETWORK_MANAGER_HEADERS}
                                ${NETWORK_MANAGER_SRCS})

target_include_directories(NetworkManagerLib PUBLIC
                                ${NETWORK_MANAGER_INCLUDE_DIRS})

target_link_libraries(NetworkManagerLib  ${Boost_LOG_SETUP_LIBRARY}
                                         ${Boost_LOG_LIBRARY})

add_executable(test_NetworkManager test_NetworkManager.cpp)

target_include_directories(test_NetworkManager PRIVATE ${NETWORK_MANAGER_INCLUDE_DIRS})

target_link_libraries(test_NetworkManager
                                ${GTEST_LIBRARIES}
                                NetworkManagerLib
                                LoggerLib
                                pthread)
