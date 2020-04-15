INCLUDE_DIRECTORIES(include)


FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


FIND_PACKAGE(Threads REQUIRED)


SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})





ADD_EXECUTABLE(server src/main.cpp)



#SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
#ADD_EXECUTABLE(test main.cpp)
#TARGET_LINK_LIBRARIES(test ${USED_LIBS})



#add_executable(tst)
#target_link_libraries (tst ${CMAKE_THREAD_LIBS_INIT}


#add_executable(tests ...)
#target_link_libraries(tests_naive ${GTEST_LIBRARIES} ...)



#enable_testing()
#add_test(tests "./tests")

