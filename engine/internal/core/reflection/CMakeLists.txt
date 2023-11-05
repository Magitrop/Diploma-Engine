cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(reflection LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(reflection STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::core::reflection ALIAS reflection)

set_target_properties(reflection PROPERTIES FOLDER "internal/core")
