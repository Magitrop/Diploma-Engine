cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(helpers LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(helpers STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::helpers ALIAS helpers)

set_target_properties(helpers PROPERTIES FOLDER "internal/helpers")
