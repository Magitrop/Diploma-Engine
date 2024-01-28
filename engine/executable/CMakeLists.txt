cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(executable LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_executable(${PROJECT_NAME} ${SOURCE} ${INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE
    engine::core::components
    engine::core::entity
    engine::core::input
    engine::core::math
    engine::core::resources
    engine::core::time

    engine::debug::drawer
    engine::debug::logging
    engine::debug::memory

    engine::editor
    engine::editor::gizmo
    engine::editor::gui
    engine::editor::projects
    engine::editor::viewport

    engine::internal::core::constants
    engine::internal::core::reflection

    engine::internal::helpers

    engine::internal::render
    engine::internal::render::glad_api

    engine::render::camera
    engine::render::lighting
    engine::render::window
    
    # externals
    glad
    glfw
    glm
    spdlog
    imgui
    imguizmo
)

set_target_properties(executable PROPERTIES FOLDER "executable")
