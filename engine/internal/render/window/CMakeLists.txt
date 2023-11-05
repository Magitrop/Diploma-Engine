cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(window_internal LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(window_internal STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::render::window ALIAS window_internal)

set_target_properties(window_internal PROPERTIES FOLDER "internal/render")
