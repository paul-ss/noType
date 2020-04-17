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

SET(CMAKE_MODULE_PATH ${SOURCE_DIR}/../modules)
FIND_PACKAGE(SFML COMPONENTS graphics window system network REQUIRED)

IF(SFML_FOUND)
    INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${ALL_HEADERS})

    ADD_EXECUTABLE(notype ${SOURCE_DIR}/main.cpp ${ALL_SOURCES})
    TARGET_LINK_LIBRARIES(notype ${SFML_LIBRARIES})
ELSE()
    MESSAGE("SFML not found")
ENDIF()
