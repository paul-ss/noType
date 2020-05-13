CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(Client)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

SET(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/application")

SET(ALL_HEADERS
    ${SOURCE_DIR}/eventManager
    ${SOURCE_DIR}/new_gui
    ${SOURCE_DIR}/exceptions
    ${SOURCE_DIR}/game
    ${SOURCE_DIR}/logger
    ${SOURCE_DIR}/soundManager
    ${SOURCE_DIR}/resources
    ${SOURCE_DIR}/sharedContext
    ${SOURCE_DIR}/stateManager
    ${SOURCE_DIR}/states
    ${SOURCE_DIR}/window
)

SET(ALL_SOURCES
    ${SOURCE_DIR}/new_gui/button.cpp
    ${SOURCE_DIR}/eventManager/eventManager.cpp
    ${SOURCE_DIR}/exceptions/exceptions.hpp
    ${SOURCE_DIR}/game/game.cpp
    ${SOURCE_DIR}/logger/logger.cpp
    ${SOURCE_DIR}/soundManager/soundManager.cpp
    ${SOURCE_DIR}/stateManager/stateManager.cpp
    ${SOURCE_DIR}/states/introState.cpp
    ${SOURCE_DIR}/states/gameState.cpp
    ${SOURCE_DIR}/states/mainMenuState.cpp
    ${SOURCE_DIR}/window/window.cpp
)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(SFML 2 REQUIRED graphics window system network audio)
FIND_PACKAGE(Boost COMPONENTS log log_setup serialization REQUIRED)

SET(Boost_USE_STATIC_LIBS ON)

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${ALL_HEADERS})

ADD_EXECUTABLE(notype ${SOURCE_DIR}/main.cpp ${ALL_SOURCES})
SET_TARGET_PROPERTIES(notype
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${SOURCE_DIR}/.."
)
TARGET_LINK_LIBRARIES(notype ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${Boost_LIBRARIES})

#ADD_SUBDIRECTORY(tests)
