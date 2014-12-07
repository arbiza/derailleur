# Minimal version of cmake
cmake_minimum_required(VERSION 2.8)

# Project name
project(derailleur)

# Compiler flags
add_definitions(-Wall -std=c++11 -Wno-unused-function)

set(CMAKE_BUILD_TYPE Release)

# Message output
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")


# Set directories
aux_source_directory(. SRC_LIST)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(INCLUDE_DIR "/usr/include/derailleur/")
set(LIBS_DIR "/usr/lib/")


# Build library
add_library(${PROJECT_NAME} SHARED  ${SRC_LIST})



### Install:
install(TARGETS derailleur DESTINATION ${LIBS_DIR})
install(FILES derailleur.hpp DESTINATION ${INCLUDE_DIR})
