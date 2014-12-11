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
set(HEADERS 
	"application.hpp"
	"log.hpp"
	"switch.hpp"
	"controller.hpp"
	"event.hpp"
	"util.hpp"
	"cli.hpp"
	)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(INCLUDE_DIR "/usr/include/")
set(HEADERS_DIR "/usr/include/derailleur/")

# Set LIBS_DIR according system architecture
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    	set( LIBS_DIR "/usr/lib64/" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set( LIBS_DIR "/usr/lib/" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )


# Build library
add_library(${PROJECT_NAME} SHARED  ${SRC_LIST})



### Install:
install(TARGETS derailleur DESTINATION ${LIBS_DIR})
install(FILES derailleur.hpp DESTINATION ${INCLUDE_DIR})
install(FILES ${HEADERS} DESTINATION ${HEADERS_DIR})

