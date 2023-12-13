cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/engine/dependencies")

include(FetchContent)
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    SOURCE_DIR ${LIBS_DIR}/spdlog
    GIT_TAG v1.12.0)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gl)
