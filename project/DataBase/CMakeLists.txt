cmake_minimum_required(VERSION 3.10)
project(mongoTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra --pedantic")

# NOTE: Не работает такой способ
# find_package(libmongocxx REQUIRED)
# find_package(libbsoncxx REQUIRED)

find_package(PkgConfig REQUIRED) 

pkg_check_modules(LIBMONGOCXX REQUIRED IMPORTED_TARGET libmongocxx)
pkg_check_modules(LIBBSONCXX REQUIRED IMPORTED_TARGET libbsoncxx)

message(STATUS PkgConfig::LIBMONGOCXX)

set(DATA_BASE_CONFIG_INCLUDE_DIR "DataBaseConfig/")
set(MONGOCXX_INIT_INCLUDE_DIR "MongoCxxInit/")

add_subdirectory(TextInfo/)
add_subdirectory(RandomName/)
add_subdirectory(PlayerInfo/)
