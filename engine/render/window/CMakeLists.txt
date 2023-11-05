cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(window LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(window STATIC ${SOURCE} ${INCLUDE})
add_library(engine::render::window ALIAS window)

set_target_properties(window PROPERTIES FOLDER "render")