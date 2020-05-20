project(PlayerInfo)

set(PLAYER_INFO_HEADERS PlayerInfo.hpp
                        PlayerInfoImpl.hpp
                     ../RandomName/RandomName.hpp
                     ../RandomName/RandomNameImpl.hpp
                     ../${MONGOCXX_INIT_HEADER}
                     ../${MONGOCXX_EXCEPTION_HEADER}
                     ../${DATA_BASE_CONFIG_HEADER})

set(PLAYER_INFO_SRCS  PlayerInfoImpl.cpp
                   ../RandomName/RandomNameImpl.cpp
                   ../${MONGOCXX_INIT_SRC}
                   ../${MONGOCXX_EXCEPTION_SRC})


add_library(PlayerInfoLib STATIC
                                    ${PLAYER_INFO_HEADERS}
                                    ${PLAYER_INFO_SRCS})

target_include_directories(PlayerInfoLib PRIVATE    ../${MONGOCXX_INIT_DIR}
                                                    ../${MONGOCXX_EXCEPTION_DIR}
                                                    ../${DATA_BASE_CONFIG_DIR}
                                                      ../RandomName/
                                                       ${CMAKE_CURRENT_SOURCE_DIR}
                                                       ${LIBMONGOCXX_INCLUDE_DIR}
                                                       ${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(PlayerInfoLib PkgConfig::LIBMONGOCXX
                                    PkgConfig::LIBBSONCXX
                                    Logger)

# Тест test_TextInfoMapper.out
add_executable(test_PlayerInfoImpl.out test_PlayerInfo.cpp)

target_include_directories(test_PlayerInfoImpl.out PRIVATE
                                                            ../${DATA_BASE_CONFIG_INCLUDE_DIR}
                                                            ../${MONGOCXX_INIT_INCLUDE_DIR}
                                                            ../${MONGOCXX_EXCEPTION_DIR}
                                                            ../${DATA_BASE_CONFIG_DIR}
                                                               ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_PlayerInfoImpl.out
                                                ${GTEST_LIBRARIES}
                                                PlayerInfoLib
                                                PkgConfig::LIBMONGOCXX
                                                PkgConfig::LIBBSONCXX
                                                pthread)

