cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME engine)
set(BUILD_SHARED_LIBS OFF)
project(${PROJECT_NAME} LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(WIN32)
    set(BUILD_PLATFORM PLATFORM_WINDOWS)
elseif(UNIX)
    set(BUILD_PLATFORM PLATFORM_LINUX)
else()
    set(BUILD_PLATFORM PLATFORM_UNKNOWN)
endif()
add_compile_definitions(${BUILD_PLATFORM})
add_compile_definitions(IS_EDITOR)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/engine)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT executable)
