cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reflection)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/constants)