cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/debug)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/editor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/internal)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libraries)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/render)