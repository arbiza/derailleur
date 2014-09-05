# Minimal version of cmake
cmake_minimum_required(VERSION 2.8)

# Project name
project(Derailleur)

# Compiler flags
add_definitions(-Wall -std=c++11)

set(CMAKE_BUILD_TYPE Release)

# Message output
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")


#set(
#    CMAKE_RUNTIME_OUTPUT_DIRECTORY
#    ${CMAKE_HOME_DIRECTORY}/bin
#    )

#set(
#    CMAKE_LIBRARY_OUTPUT_DIRECTORY
#    ${CMAKE_HOME_DIRECTORY}/lib
#    )

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/lib
    )


add_library(derailleur SHARED controller.cpp switch.cpp switch.cpp)

#install(TARGETS derailleur RUNTIME_OUTPUT_DIRECTORY ${DERAILLEUR_BINARY_DIR}/bin)

#set_target_properties(derailleur PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# EXAMPLES

#install(TARGETS test DESTINATION lib)
#install(FILES test.h DESTINATION include)

#SET(SOURCES source1 source2 source3)
#FOREACH(source ${SOURCES})
#  ADD_EXECUTABLE(${source} ${source}.c)
#ENDFOREACH(source)

#include_directories("${CMAKE_SOURCE_DIR}/namer")


#FIND_PATH(PYTHON_INCLUDE_PATH Python.h
#  /usr/include
#  /usr/local/include)
