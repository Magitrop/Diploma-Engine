cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(editor LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(editor STATIC ${SOURCE} ${INCLUDE})
add_library(engine::editor ALIAS editor)

set_target_properties(editor PROPERTIES FOLDER "editor")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gizmo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/viewport)

