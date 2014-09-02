### Project options (set your options here)


project(Testing_Derailleur_Controller)
set(EXECUTABLE testing)
set(SOURCES main.cpp)


# -----------------------------------------


# Minimal version of cmake
cmake_minimum_required(VERSION 2.8)


# Compiler flags
add_definitions(-Wall -std=c++11)


# Default build type
set(CMAKE_BUILD_TYPE Release)


# Messages output
message("Building project ${project}")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")


# Add derailleur library
add_library(libderailleur SHARED IMPORTED)
set_property(TARGET libderailleur PROPERTY IMPORTED_LOCATION /home/lucas/projetos/derailleur/controller/build/lib/libderailleur.so)


# Executable
add_executable(${EXECUTABLE} ${SOURCES})


# Link libraries
target_link_libraries(${EXECUTABLE} fluid_base fluid_msg libderailleur)
