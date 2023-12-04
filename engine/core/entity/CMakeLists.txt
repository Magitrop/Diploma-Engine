cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(entity LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(entity STATIC ${SOURCE} ${INCLUDE})
add_library(engine::core::entity ALIAS entity)

set_target_properties(entity PROPERTIES FOLDER "core")