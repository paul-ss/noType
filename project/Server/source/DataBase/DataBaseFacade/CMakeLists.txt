project(DataBaseFacade)

find_package(GTest REQUIRED)
include_directories(${GTEST_INLCUDE_DIRS})

set(DATA_BASE_FACADE_HEADERS  ../DataBaseConfig/DataBaseConfig.hpp
                              ../TextInfo/TextInfo.hpp
                              ../TextInfo/TextInfoImpl.hpp
                              ../PlayerInfo/PlayerInfo.hpp
                              ../PlayerInfo/PlayerInfoImpl.hpp
                              ../RandomName/RandomName.hpp
                              ../RandomName/RandomNameImpl.hpp
                              ../MongoCxxInit/MongoCxxInit.hpp
                              ../DataBaseFacade/DataBaseFacade.hpp)


set(DATA_BASE_FACADE_INCLUDE_DIRS   ../DataBaseConfig/
                                    ../TextInfo/
                                    ../TextInfo/
                                    ../PlayerInfo/
                                    ../PlayerInfo/
                                    ../RandomName/
                                    ../RandomName/
                                    ../MongoCxxInit/
                                    ../DataBaseFacade/)

set(DATA_BASE_FACADE_SOURCE   ../TextInfo/TextInfoImpl.cpp
                              ../PlayerInfo/PlayerInfoImpl.cpp
                              ../RandomName/RandomNameImpl.cpp
                              ../MongoCxxInit/MongoCxxInit.cpp
                              ../DataBaseFacade/DataBaseFacade.cpp)

add_library(DataBaseFacadeLib STATIC ${DATA_BASE_FACADE_HEADERS} ${DATA_BASE_FACADE_SOURCE})
target_include_directories(DataBaseFacadeLib PUBLIC ${DATA_BASE_FACADE_INCLUDE_DIRS})
target_link_libraries(DataBaseFacadeLib  PkgConfig::LIBMONGOCXX
                                         PkgConfig::LIBBSONCXX)

# Тест test_PlayerInfo.out
add_executable(test_DataBaseFacade.out  test_DataBaseFacade.cpp ${DATA_BASE_FACADE_SOURCE})

target_link_libraries(test_DataBaseFacade.out
                                          DataBaseFacadeLib
                                          ${GTEST_LIBRARIES}
                                          pthread)
