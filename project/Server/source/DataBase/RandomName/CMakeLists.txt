project(RandomName)

set(RANDOM_NAME_HEADERS RandomName.hpp
                        RandomNameImpl.hpp
                     ../${MONGOCXX_INIT_HEADER}
                     ../${MONGOCXX_EXCEPTION_HEADER}
                     ../${DATA_BASE_CONFIG_HEADER})

set(RANDOM_NAME_SRCS  RandomNameImpl.cpp
                   ../${MONGOCXX_INIT_SRC}
                   ../${MONGOCXX_EXCEPTION_SRC})


add_library(RandomNameLib STATIC
                                    ${RANDOM_NAME_HEADERS}
                                    ${RANDOM_NAME_SRCS})

target_include_directories(RandomNameLib PRIVATE    ../${MONGOCXX_INIT_DIR}
                                                    ../${MONGOCXX_EXCEPTION_DIR}
                                                    ../${DATA_BASE_CONFIG_DIR}
                                                       ${CMAKE_CURRENT_SOURCE_DIR}
                                                       ${LIBMONGOCXX_INCLUDE_DIR}
                                                       ${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(RandomNameLib PkgConfig::LIBMONGOCXX
                                    PkgConfig::LIBBSONCXX
                                    LoggerLib)

# Тест test_TextInfoMapper.out
add_executable(test_RandomNameImpl.out test_RandomNameImpl.cpp)

target_include_directories(test_RandomNameImpl.out PRIVATE
                                                            ../${DATA_BASE_CONFIG_INCLUDE_DIR}
                                                            ../${MONGOCXX_INIT_INCLUDE_DIR}
                                                            ../${MONGOCXX_EXCEPTION_DIR}
                                                            ../${DATA_BASE_CONFIG_DIR}
                                                               ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_RandomNameImpl.out
                                            ${GTEST_LIBRARIES}
                                            RandomNameLib
                                            PkgConfig::LIBMONGOCXX
                                            PkgConfig::LIBBSONCXX
                                            pthread)

