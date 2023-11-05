cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(gui LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(gui STATIC ${SOURCE} ${INCLUDE})
add_library(engine::editor::gui ALIAS gui)

set_target_properties(gui PROPERTIES FOLDER "editor")