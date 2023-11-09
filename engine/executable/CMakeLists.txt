cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(executable LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_executable(${PROJECT_NAME} ${SOURCE} ${INCLUDE})

find_package(fmt REQUIRED PATHS "${CMAKE_SOURCE_DIR}/engine/dependencies/fmt")
find_package(spdlog REQUIRED PATHS "${CMAKE_SOURCE_DIR}/engine/dependencies/spdlog")
target_link_libraries(${PROJECT_NAME} PRIVATE 
    engine::core::components
    engine::core::input
    engine::core::math
    engine::core::objects
    engine::core::resources
    engine::core::time

    engine::debug::drawer
    engine::debug::logging

    engine::editor::gizmo
    engine::editor::gui
    engine::editor::initialization

    engine::render::camera
    engine::render::graphics
    engine::render::lighting
    engine::render::window
	
	engine::internal::core::projects
	engine::internal::core::reflection

	engine::internal::helpers

    engine::internal::render::window
	engine::internal::render::gl::glfw
	engine::internal::render::gl::glad
	engine::internal::render::gl::glm

    # externals
    spdlog
)

set_target_properties(executable PROPERTIES FOLDER "executable")
