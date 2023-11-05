cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/time)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/objects)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/math)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/input)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/components)