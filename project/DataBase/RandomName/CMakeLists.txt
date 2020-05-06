project(test_RandomNameImpl)

find_package(GTest REQUIRED)
include_directories(${GTEST_INLCUDE_DIRS})

# Тест test_TextInfoMapper.out
add_executable(test_RandomNameImpl.out test_RandomNameImpl.cpp
                                       RandomNameImpl.cpp)

target_include_directories(test_RandomNameImpl.out PRIVATE
                                                           ../${DATA_BASE_CONFIG_INCLUDE_DIR}
                                                              ${CMAKE_CURRENT_SOURCE_DIR}
                                                              ${LIBMONGOCXX_INCLUDE_DIR}
                                                              ${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(test_RandomNameImpl.out
                                              ${GTEST_LIBRARIES}
                                              PkgConfig::LIBMONGOCXX 
                                              PkgConfig::LIBBSONCXX
                                              pthread)
