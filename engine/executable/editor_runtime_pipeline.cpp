#include "editor_runtime_pipeline.h"

#include <engine/editor/initialization/editor_initializer.h>
#include <engine/internal/render/gl/glad/include/glad/glad.h>
#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>
#include <engine/internal/render/window/window_manager.h>

using EditorRuntimePipeline = engine::executable::EditorRuntimePipeline;

void EditorRuntimePipeline::initialize()
{
	initializeGL();
	initializeEditor();
	//initializeWindowManager();
}

void EditorRuntimePipeline::runtime()
{
}

void EditorRuntimePipeline::initializeEditor()
{

}

const engine::executable::EditorPipelineStorage* EditorRuntimePipeline::storage() const
{
	return &m_storage;
}

void EditorRuntimePipeline::initializeGL()
{
	glfwInit();
	gladLoadGL();
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
