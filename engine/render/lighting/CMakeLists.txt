cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(lighting LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(lighting STATIC ${SOURCE} ${INCLUDE})
add_library(engine::render::lighting ALIAS lighting)

set_target_properties(lighting PROPERTIES FOLDER "render")