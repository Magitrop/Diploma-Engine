#include "editor_runtime_pipeline.h"

#include <engine/internal/render/gl/glad/include/glad/glad.h>
#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>
#include <engine/internal/render/window/window_manager.h>
#include <engine/executable/runtime.h>

#include <iostream>

using EditorRuntimePipeline = engine::executable::EditorRuntimePipeline;
using WindowManager = engine::internal::render::WindowManager;
using Window = engine::render::Window;

EditorRuntimePipeline::EditorRuntimePipeline() 
	: m_editorWindow(nullptr)
{}

void EditorRuntimePipeline::initialize()
{
	initializeGLFW();
	initializeWindowManager();
	initializeEditor();
}

void EditorRuntimePipeline::run()
{
	// creating editor winow
	m_editorWindow = m_windowManager->createWindow(400, 400, "Editor");
	m_windowManager->setWindowAsCurrentContext(m_editorWindow);

	initializeGraphicAPI();

	m_isRunning = true;

	while (isRunning())
	{

	}
}

void EditorRuntimePipeline::initializeEditor()
{

}
