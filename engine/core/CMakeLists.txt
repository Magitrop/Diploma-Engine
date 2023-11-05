cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(core LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_library(core STATIC ${CMAKE_CURRENT_SOURCE_DIR}/null.cpp)
add_library(engine::core ALIAS core)