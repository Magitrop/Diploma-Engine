cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(viewport LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(viewport STATIC ${SOURCE} ${INCLUDE})
add_library(engine::editor::viewport ALIAS viewport)

set_target_properties(viewport PROPERTIES FOLDER "editor")