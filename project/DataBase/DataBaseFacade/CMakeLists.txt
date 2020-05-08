project(test_DataBaseFacade)

find_package(GTest REQUIRED)
include_directories(${GTEST_INLCUDE_DIRS})

# Тест test_PlayerInfo.out
add_executable(test_DataBaseFacade.out  test_DataBaseFacade.cpp
                                             DataBaseFacade.cpp
                                   ../TextInfo/TextInfoImpl.cpp
                               ../PlayerInfo/PlayerInfoImpl.cpp
                               ../RandomName/RandomNameImpl.cpp
               ../${MONGOCXX_INIT_INCLUDE_DIR}/MongoCxxInit.cpp)

target_include_directories(test_DataBaseFacade.out PRIVATE
                                                      
                                                      ../${DATA_BASE_CONFIG_INCLUDE_DIR}
                                                      ../${MONGOCXX_INIT_INCLUDE_DIR}
                                                         ${CMAKE_CURRENT_SOURCE_DIR}
                                                         ${LIBMONGOCXX_INCLUDE_DIR}
                                                         ${LIBBSONCXX_INCLUDE_DIR}
                                                      ../PlayerInfo/
                                                      ../RandomName/
                                                      ../TextInfo/)

target_link_libraries(test_DataBaseFacade.out
                                          ${GTEST_LIBRARIES}
                                          PkgConfig::LIBMONGOCXX 
                                          PkgConfig::LIBBSONCXX
                                          pthread)
