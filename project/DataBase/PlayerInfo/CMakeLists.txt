project(test_PlayerInfo)

find_package(GTest REQUIRED)
include_directories(${GTEST_INLCUDE_DIRS})

# Тест test_PlayerInfo.out
add_executable(test_PlayerInfo.out test_PlayerInfo.cpp
                                    PlayerInfoImpl.cpp
                      ../RandomName/RandomNameImpl.cpp
      ../${MONGOCXX_INIT_INCLUDE_DIR}/MongoCxxInit.cpp)

target_include_directories(test_PlayerInfo.out PRIVATE
                                                      ../${DATA_BASE_CONFIG_INCLUDE_DIR}
                                                      ../${MONGOCXX_INIT_INCLUDE_DIR}
                                                         ${CMAKE_CURRENT_SOURCE_DIR}
                                                         ${LIBMONGOCXX_INCLUDE_DIR}
                                                         ${LIBBSONCXX_INCLUDE_DIR}
                                                      ../RandomName/)

target_link_libraries(test_PlayerInfo.out
                                          ${GTEST_LIBRARIES}
                                          PkgConfig::LIBMONGOCXX 
                                          PkgConfig::LIBBSONCXX
                                          pthread)
