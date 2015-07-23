cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR DistroHash)
project(${PROJECT_NAME_STR} C CXX)

set(CMAKE_CXX_FLAGS "-g -Wall -std=gnu++0x")
find_package(Threads REQUIRED)

#if(CMAKE_COMPILER_IS_GNUCXX)
    #list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
    #add_definitions(-Wall -ansi -Wno-deprecated -pthread)
#endif()

#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(COMMON_SOURCES ${PROJECT_SOURCE_DIR}/src)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

#-------------------
# Module source
#-------------------
#set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_impl)

#include_directories(${COMMON_INCLUDES})
#add_library(${PROJECT_LIB_NAME} ${SRC_FILES})
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
add_subdirectory(${EXT_PROJECTS_DIR}/zmqpp)
add_subdirectory(${EXT_PROJECTS_DIR}/json11)

#-------------------
# Test
#-------------------
add_definitions(${MSVC_COMPILER_DEFS})
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})
include_directories(${EXT_PROJECTS_DIR}/zmqpp/src)
include_directories(${EXT_PROJECTS_DIR}/json11)
#add_library(${PROJECT_TEST_NAME} ${SRC_FILES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
set(TESTING_SRC_FILES ${SRC_FILES})
list(REMOVE_ITEM TESTING_SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES} ${TESTING_SRC_FILES})
add_executable(${PROJECT_NAME_STR} ${SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
add_dependencies(${PROJECT_TEST_NAME} json11)
#add_dependencies(${PROJECT_TEST_NAME} zmqpp)


target_link_libraries(${PROJECT_TEST_NAME}
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    )

target_link_libraries(${PROJECT_NAME_STR} json11)

target_link_libraries(${PROJECT_NAME_STR} zmqpp)

target_link_libraries(${PROJECT_TEST_NAME} json11)

target_link_libraries(${PROJECT_TEST_NAME} zmqpp)

target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_test(test1 ${PROJECT_TEST_NAME})
