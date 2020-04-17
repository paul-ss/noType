SET(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/application")

SET(ALL_HEADERS
    ${SOURCE_DIR}/eventManager
    ${SOURCE_DIR}/window
    ${SOURCE_DIR}/game
)

SET(ALL_SOURCES
    ${SOURCE_DIR}/eventManager/eventManager.cpp
    ${SOURCE_DIR}/window/window.cpp
    ${SOURCE_DIR}/game/game.cpp
)

message(WARNING "cmake_modules: ${CMAKE_CURRENT_SOURCE_DIR}/cmake")

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED graphics window system network)

message(WARNING "SFML_INCLUDE_DIR ${SFML_INCLUDE_DIR}")

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALL_HEADERS})

ADD_EXECUTABLE(notype ${SOURCE_DIR}/main.cpp ${ALL_SOURCES})
TARGET_LINK_LIBRARIES(notype ${SFML_LIBRARIES})
