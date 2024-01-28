cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/engine/dependencies")

include(FetchContent)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	SOURCE_DIR ${LIBS_DIR}/glfw)
FetchContent_MakeAvailable(glfw)
include_directories(${LIBS_DIR}/glfw/include)
set_target_properties(glfw PROPERTIES FOLDER "dependencies")

# glad
#FetchContent_Declare(glad
#	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
#	SOURCE_DIR ${LIBS_DIR}/glad)
#FetchContent_MakeAvailable(glad)
#include_directories(${GLAD_BINARY_DIR}/include)
#set_target_properties(glad PROPERTIES FOLDER "dependencies")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)

# glm
FetchContent_Declare(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	SOURCE_DIR ${LIBS_DIR}/glm)
FetchContent_MakeAvailable(glm)
include_directories(${LIBS_DIR}/glm)
set_target_properties(glm PROPERTIES FOLDER "dependencies")

# stb
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    SOURCE_DIR ${LIBS_DIR}/stb)
FetchContent_MakeAvailable(stb)
include_directories(${LIBS_DIR}/stb)
#set_target_properties(stb PROPERTIES FOLDER "dependencies")

# spdlog
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    SOURCE_DIR ${LIBS_DIR}/spdlog
    GIT_TAG v1.12.0)
FetchContent_MakeAvailable(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "dependencies")

# imguizmo
#FetchContent_Declare(imguizmo
#    GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
#    SOURCE_DIR ${LIBS_DIR}/imguizmo)
#FetchContent_MakeAvailable(imguizmo)
#set_target_properties(imguizmo PROPERTIES FOLDER "dependencies")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imguizmo)

# imgui
set(IMGUI_DIR ${LIBS_DIR}/imgui)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    SOURCE_DIR ${LIBS_DIR}/imgui
    GIT_TAG v1.90)
FetchContent_MakeAvailable(imgui)
set(IMGUI_INCLUDES
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_draw.cpp)
add_library(imgui STATIC ${IMGUI_INCLUDES} ${IMGUI_SOURCES})
set_target_properties(imgui PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/imgui-build
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/imgui-build
    FOLDER "dependencies")
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_link_libraries(imgui PRIVATE glad glfw)
