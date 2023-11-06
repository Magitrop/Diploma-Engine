cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/helpers)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/render)
