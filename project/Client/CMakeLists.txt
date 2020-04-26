CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(Client)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

SET(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/application")

SET(ALL_HEADERS
    ${SOURCE_DIR}/utils
    ${SOURCE_DIR}/eventManager
    ${SOURCE_DIR}/resourceManager
    ${SOURCE_DIR}/window
    ${SOURCE_DIR}/sharedContext
    ${SOURCE_DIR}/states
    ${SOURCE_DIR}/stateManager
    ${SOURCE_DIR}/logger
    ${SOURCE_DIR}/exceptions
    ${SOURCE_DIR}/music
    ${SOURCE_DIR}/game
)

SET(ALL_SOURCES
    ${SOURCE_DIR}/eventManager/eventManager.cpp
    ${SOURCE_DIR}/window/window.cpp
    ${SOURCE_DIR}/states/introState.cpp
    ${SOURCE_DIR}/states/mainMenuState.cpp
    #${SOURCE_DIR}/states/pauseState.cpp
    #${SOURCE_DIR}/states/gameState.cpp
    ${SOURCE_DIR}/stateManager/stateManager.cpp
    ${SOURCE_DIR}/music/soundManager.cpp
    ${SOURCE_DIR}/game/game.cpp
)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(SFML REQUIRED graphics window system network audio)

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALL_HEADERS})

ADD_EXECUTABLE(notype ${SOURCE_DIR}/main.cpp ${ALL_SOURCES})
SET_TARGET_PROPERTIES(notype
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${SOURCE_DIR}/.."
)
TARGET_LINK_LIBRARIES(notype ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
