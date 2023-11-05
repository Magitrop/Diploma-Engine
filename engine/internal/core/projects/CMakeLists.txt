cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(projects LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(projects STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::core::projects ALIAS projects)

set_target_properties(projects PROPERTIES FOLDER "internal/core")
