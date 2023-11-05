cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_executable(${PROJECT_NAME} 
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    engine::core::components
    engine::core::input
	
    engine::render::camera
    engine::render::graphics
    engine::render::lighting
    engine::render::window
	
	engine::internal::render::gl::glfw
	engine::internal::render::gl::glad
	engine::internal::render::gl::glm
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gizmo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/initialization)
