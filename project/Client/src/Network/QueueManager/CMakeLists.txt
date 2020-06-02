cmake_minimum_required(VERSION 3.10)
project(QueueManager)

set(QUEUE_MANAGER_INCLUDE_DIRS
                                ../Message/
                                ../PlayerInfo/
                                ${CMAKE_INCLUDE_CURRENT_DIR})

set(QUEUE_MANAGER_HEADERS
                                ../PlayerInfo/PlayerInfo.hpp
                                ../Message/Message.hpp
                                QueueManager.hpp)

set(QUEUE_MANAGER_SRCS
                                ../Message/Message.cpp
                                QueueManager.cpp)



add_library(QueueManagerLib STATIC
                                ${QUEUE_MANAGER_HEADERS}
                                ${QUEUE_MANAGER_SRCS})

target_include_directories(QueueManagerLib PUBLIC
                                ${QUEUE_MANAGER_INCLUDE_DIRS})

#add_executable(test_QueueManager test_QueueManager.cpp)
#
#target_link_libraries(test_QueueManager
#                                ${GTEST_LIBRARIES}
#                                QueueManagerLib
#                                pthread)
