cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(glad_api LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(glad_api STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::render::glad_api ALIAS glad_api)

set_target_properties(glad_api PROPERTIES FOLDER "internal/render/glad")
