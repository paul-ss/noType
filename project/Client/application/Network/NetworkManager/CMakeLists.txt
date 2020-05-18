cmake_minimum_required(VERSION 3.10)
project(NetworkManager)


set(NETWORK_MANAGER_INCLUDE_DIRS
                                ../Message/
                                ../PlayerInfo/
                                ../MessageParser/
                                ../QueueManager/
                                ${CMAKE_INCLUDE_CURRENT_DIR})

set(NETWORK_MANAGER_HEADERS
                                ../Message/Message.hpp
                                ../PlayerInfo/PlayerInfo.hpp
                                ../MessageParser/MessageParser.hpp
                                ../MessageParser/MessageParserConfig.hpp
                                ../QueueManager/QueueManager.hpp
                                NetworkManager.Hpp
                                NetworkManagerConfig.hpp)

set(NETWORK_MANAGER_SRCS
                                ../Message/Message.cpp
                                ../MessageParser/MessageParser.cpp
                                ../QueueManager/QueueManager.cpp
                                NetworkManager.cpp)


add_library(NetworkManagerLib STATIC
                                ${NETWORK_MANAGER_HEADERS}
                                ${NETWORK_MANAGER_SRCS})

target_include_directories(NetworkManagerLib PUBLIC
                                ${NETWORK_MANAGER_INCLUDE_DIRS})

#add_executable(test_MessageParser test_MessageParser.cpp)
#
#target_link_libraries(test_Message
#                                    ${GTEST_LIBRARIES}
#                                    MessageParserLib
#                                    pthread)
