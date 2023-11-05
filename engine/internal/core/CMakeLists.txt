cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(core_internal LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_library(core_internal STATIC ${CMAKE_CURRENT_SOURCE_DIR}/null.cpp)
add_library(engine::internal::core ALIAS core_internal)