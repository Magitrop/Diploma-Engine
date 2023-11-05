cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(render LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_library(render STATIC ${CMAKE_CURRENT_SOURCE_DIR}/null.cpp)
add_library(engine::render ALIAS render)
