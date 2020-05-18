project(TextInfo)

set(TEXT_INFO_HEADERS TextInfo.hpp
                     TextInfoImpl.hpp
                  ../${MONGOCXX_INIT_HEADER}
                  ../${MONGOCXX_EXCEPTION_HEADER}
                  ../${DATA_BASE_CONFIG_HEADER})

set(TEXT_INFO_SRCS TextInfoImpl.cpp
                  ../${MONGOCXX_INIT_SRC}
                  ../${MONGOCXX_EXCEPTION_SRC})


add_library(TextInfoLib STATIC
                                ${TEXT_INFO_HEADERS}
                                ${TEXT_INFO_SRCS})

target_include_directories(TextInfoLib PRIVATE   ../${MONGOCXX_INIT_DIR}
                                                 ../${MONGOCXX_EXCEPTION_DIR}
                                                 ../${DATA_BASE_CONFIG_DIR}
                                                    ${CMAKE_CURRENT_SOURCE_DIR}
                                                    ${LIBMONGOCXX_INCLUDE_DIR}
                                                    ${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(TextInfoLib PkgConfig::LIBMONGOCXX
                                  PkgConfig::LIBBSONCXX)

# Тест test_TextInfoMapper.out
add_executable(test_TextInfo.out test_TextInfoImpl.cpp)

target_include_directories(test_TextInfo.out PRIVATE
                                                      ../${DATA_BASE_CONFIG_INCLUDE_DIR}
                                                      ../${MONGOCXX_INIT_INCLUDE_DIR}
                                                      ../${MONGOCXX_EXCEPTION_DIR}
                                                      ../${DATA_BASE_CONFIG_DIR}
                                                         ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_TextInfo.out
                                        ${GTEST_LIBRARIES}
                                        TextInfoLib
                                        PkgConfig::LIBMONGOCXX 
                                        PkgConfig::LIBBSONCXX
                                        pthread)
