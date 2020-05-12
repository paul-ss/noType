project(DataBase)


find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMONGOCXX REQUIRED IMPORTED_TARGET libmongocxx)
pkg_check_modules(LIBBSONCXX REQUIRED IMPORTED_TARGET libbsoncxx)

find_package(GTest REQUIRED)
#include_directories(${GTEST_INLCUDE_DIRS})

#PavelS:
#INCLUDE_DIRECTORIES(${LIBMONGOCXX_INCLUDE_DIRS})

set(DATA_BASE_CONFIG_DIR "DataBaseConfig")
set(DATA_BASE_CONFIG_HEADER "${DATA_BASE_CONFIG_DIR}/DataBaseConfig.hpp")

set(MONGOCXX_INIT_DIR "MongoCxxInit")
set(MONGOCXX_INIT_HEADER "${MONGOCXX_INIT_DIR}/MongoCxxInit.hpp")
set(MONGOCXX_INIT_SRC "${MONGOCXX_INIT_DIR}/MongoCxxInit.cpp")

set(MONGOCXX_EXCEPTION_DIR "DataBaseException")
set(MONGOCXX_EXCEPTION_HEADER "${MONGOCXX_EXCEPTION_DIR}/DataBaseException.hpp")
set(MONGOCXX_EXCEPTION_SRC "${MONGOCXX_EXCEPTION_DIR}/DataBaseException.cpp")


# NOTE(vendroid): Директории с тестами
add_subdirectory(TextInfo/)
add_subdirectory(RandomName/)
add_subdirectory(PlayerInfo/)
add_subdirectory(DataBaseInit/)
add_subdirectory(DataBaseFacade/)
