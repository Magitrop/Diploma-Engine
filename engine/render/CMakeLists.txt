cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/camera)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lighting)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/window)