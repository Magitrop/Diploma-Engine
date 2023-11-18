cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(memory LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(memory STATIC ${SOURCE} ${INCLUDE})
add_library(engine::debug::memory ALIAS memory)

set_target_properties(memory PROPERTIES FOLDER "debug")