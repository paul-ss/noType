project(DataBaseInit)

# NOTE(vendroid): Драйвер загрузки начальных данных в базу данных
add_executable(DataBaseInit.out DataBaseInit.cpp)
target_include_directories(DataBaseInit.out PRIVATE ${LIBMONGOCXX_INCLUDE_DIR}
                                                    ${LIBBSONCXX_INCLUDE_DIR})
target_link_libraries(DataBaseInit.out PkgConfig::LIBMONGOCXX PkgConfig::LIBBSONCXX)
