cmake_minimum_required(VERSION 3.10)
project(MessageParser)

set(MESSAGE_PARSER_INCLUDE_DIRS
                                ../Message/
                                ../PlayerInfo/
                                ${CMAKE_INCLUDE_CURRENT_DIR})

set(MESSAGE_PARSER_HEADERS
                                ../PlayerInfo/PlayerInfo.hpp
                                ../Message/Message.hpp
                                MessageParser.hpp
                                MessageParserConfig.hpp)

set(MESSAGE_PARSER_SRCS
                                ../Message/Message.cpp
                                MessageParser.cpp)



add_library(MessageParserLib STATIC
                                ${MESSAGE_PARSER_HEADERS}
                                ${MESSAGE_PARSER_SRCS})

target_include_directories(MessageParserLib PUBLIC
                                ${MESSAGE_PARSER_INCLUDE_DIRS})

#add_executable(test_MessageParser test_MessageParser.cpp)
#
#target_link_libraries(test_Message
#                                    ${GTEST_LIBRARIES}
#                                    MessageParserLib
#                                    pthread)
