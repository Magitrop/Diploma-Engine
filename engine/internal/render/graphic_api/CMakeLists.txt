cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(graphic_api LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(graphic_api STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::render::graphic_api ALIAS graphic_api)

set_target_properties(graphic_api PROPERTIES FOLDER "internal/render")
