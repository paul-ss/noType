cmake_minimum_required(VERSION 3.10)
project(Network)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

add_subdirectory(Message)
add_subdirectory(QueueManager)
add_subdirectory(MessageParser)
