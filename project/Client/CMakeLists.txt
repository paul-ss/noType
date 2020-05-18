CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(Client)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

SET(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/application")

SET(SOURCE_THIRD_PARTY "${CMAKE_CURRENT_SOURCE_DIR}/thirdPartyLib")

SET(THIRD_PARTY_INCLUDE
    ${SOURCE_THIRD_PARTY}/animation
    ${SOURCE_THIRD_PARTY}/animation
    ${SOURCE_THIRD_PARTY}/progressBar
    ${SOURCE_THIRD_PARTY}/progressBar
    ${SOURCE_THIRD_PARTY}/colorString
)

ADD_LIBRARY(animation STATIC
    ${SOURCE_THIRD_PARTY}/animation/animation.hpp
    ${SOURCE_THIRD_PARTY}/animation/animation.cpp
    ${SOURCE_THIRD_PARTY}/animation/animatedSprite.hpp
    ${SOURCE_THIRD_PARTY}/animation/animatedSprite.hpp
)

ADD_LIBRARY(progressBar STATIC
    ${SOURCE_THIRD_PARTY}/progressBar/Common.hpp
    ${SOURCE_THIRD_PARTY}/progressBar/selbaProgressBar.hpp
    ${SOURCE_THIRD_PARTY}/progressBar/selbaProgressBar.cpp
)

ADD_LIBRARY(colorString STATIC
    ${SOURCE_THIRD_PARTY}/colorString/RichText.hpp
    ${SOURCE_THIRD_PARTY}/colorString/RichText.cpp
)

SET(ALL_HEADERS
    ${SOURCE_DIR}/utils
    ${SOURCE_DIR}/eventManager
    ${SOURCE_DIR}/ui
    ${SOURCE_DIR}/exceptions
    ${SOURCE_DIR}/game
    ${SOURCE_DIR}/logger
    ${SOURCE_DIR}/soundManager
    ${SOURCE_DIR}/resources
    ${SOURCE_DIR}/sharedContext
    ${SOURCE_DIR}/stateManager
    ${SOURCE_DIR}/states
    ${SOURCE_DIR}/window
    ${SOURCE_DIR}/Network/
    ${SOURCE_DIR}/Network/QueueManager
    ${SOURCE_DIR}/Network/NetworkManager
    ${SOURCE_DIR}/Network/Message
    ${SOURCE_DIR}/Network/MessageParser
    ${SOURCE_DIR}/Network/PlayerInfo
)

SET(ALL_SOURCES
    ${SOURCE_DIR}/ui/baseElement.cpp
    ${SOURCE_DIR}/ui/smartString.cpp
    ${SOURCE_DIR}/ui/label.cpp
    ${SOURCE_DIR}/eventManager/eventManager.cpp
    ${SOURCE_DIR}/exceptions/exceptions.hpp
    ${SOURCE_DIR}/game/game.cpp
    ${SOURCE_DIR}/logger/logger.cpp
    ${SOURCE_DIR}/soundManager/soundManager.cpp
    ${SOURCE_DIR}/stateManager/stateManager.cpp
    ${SOURCE_DIR}/states/introState.cpp
    ${SOURCE_DIR}/states/gameState.hpp
    ${SOURCE_DIR}/states/afterGameState.hpp
    ${SOURCE_DIR}/states/gameState.cpp
    ${SOURCE_DIR}/states/mainMenuState.cpp
    ${SOURCE_DIR}/states/afterGameState.cpp
    ${SOURCE_DIR}/states/beforeGameState.cpp
    ${SOURCE_DIR}/window/window.cpp
    ${SOURCE_DIR}/Network/QueueManager/QueueManager.cpp
    ${SOURCE_DIR}/Network/NetworkManager/NetworkManager.cpp
    ${SOURCE_DIR}/Network/Message/Message.cpp
    ${SOURCE_DIR}/Network/MessageParser/MessageParser.cpp
)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(SFML 2 REQUIRED graphics window system network audio)
FIND_PACKAGE(Boost COMPONENTS log log_setup serialization REQUIRED)

INCLUDE_DIRECTORIES(${THIRD_PARTY_INCLUDE})
INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALL_HEADERS})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

ADD_EXECUTABLE(notype ${SOURCE_DIR}/main.cpp ${ALL_SOURCES})

SET_TARGET_PROPERTIES(notype
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${SOURCE_DIR}/.."
)

ADD_SUBDIRECTORY(${SOURCE_DIR}/Network)

TARGET_LINK_LIBRARIES(notype
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}
    ${Boost_LIBRARIES}
    NetworkManagerLib
    colorString
    progressBar
    animation)


#ADD_SUBDIRECTORY(tests)