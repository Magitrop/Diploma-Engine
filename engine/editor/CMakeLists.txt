cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gizmo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/viewport)

