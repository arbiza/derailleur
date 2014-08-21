# Minimal version of cmake
cmake_minimum_required(VERSION 2.6)

# Project name
project(ThingFlow)

# Compiler flags
add_definitions(-Wall -std=c++11)

# Message output
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

# Compiling the main file
add_executable(thingflow main.cpp)
