cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(render_internal LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(gl/glfw)
add_subdirectory(gl/glad)
add_subdirectory(gl/glm)

add_library(render_internal STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/null.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/window/window_manager.cpp
)
add_library(engine::internal::render          ALIAS render_internal)
add_library(engine::internal::render::glfw    ALIAS glfw)
add_library(engine::internal::render::glad    ALIAS glad)
add_library(engine::internal::render::glm     ALIAS glm)
