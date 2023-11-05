cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(editor LANGUAGES CXX)
message(STATUS "Processing ${CMAKE_CURRENT_SOURCE_DIR}...")

add_executable(${PROJECT_NAME} 
	${CMAKE_CURRENT_SOURCE_DIR}/initialization/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    engine::core
	
    engine::render
	engine::internal::render::glfw
	engine::internal::render::glad
	engine::internal::render::glm
)