cmake_minimum_required(VERSION 3.10)
project(Message)

set(MESSAGE_INCLUDE_DIRS
                            ../PlayerInfo/
                            ${CMAKE_INCLUDE_CURRENT_DIR})

set(MESSAGE_HEADERS
                    ../PlayerInfo/PlayerInfo.hpp
                    Message.hpp)

set(MESSAGE_SRCS Message.cpp)


add_library(MessageLib STATIC
                                ${MESSAGE_HEADERS}
                                ${MESSAGE_SRCS})
target_include_directories(MessageLib PUBLIC
                                                ${MESSAGE_INCLUDE_DIRS})

#add_executable(test_Message test_Message.cpp)
#target_include_directories(test_Message PUBLIC
#                                                ${MESSAGE_INCLUDE_DIRS}
#                                                ${GTEST_INClUDE_DIRS})
#
#target_link_libraries(test_Message
#                                    ${GTEST_LIBRARIES}
#                                    MessageLib
#                                    pthread)
