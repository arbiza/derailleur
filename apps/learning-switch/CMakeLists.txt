### Project options (set your options here)


project(Learning_Switch)
set(EXECUTABLE learning-switch)
set(SOURCES main.cpp learning_switch.cpp)


# -----------------------------------------


# Minimal version of cmake
cmake_minimum_required(VERSION 2.8)


# Compiler flags
add_definitions(-std=c++11 -Wall -Wno-unused-function)


# Default build type
set(CMAKE_BUILD_TYPE Release)


# Messages output
message("Building project ${project}")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")


# Executable
add_executable(${EXECUTABLE} ${SOURCES})


# Link libraries
target_link_libraries(${EXECUTABLE} fluid_base fluid_msg pthread derailleur)
