cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(graphics LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(graphics STATIC ${SOURCE} ${INCLUDE})
add_library(engine::internal::render::graphics ALIAS graphics)

set_target_properties(graphics PROPERTIES FOLDER "internal/render")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)
