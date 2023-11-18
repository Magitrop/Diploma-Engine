#include "editor_runtime_pipeline.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>
#include <engine/internal/render/window/window_manager.h>
#include <engine/executable/runtime.h>

#include <iostream>

namespace engine
{
	EditorRuntimePipeline::EditorRuntimePipeline()
		: m_editorWindow(nullptr)
	{}

	void EditorRuntimePipeline::initialize()
	{
		MEMORY_GUARD;

		initializeGLFW();
		initializeWindowManager();
		initializeEditor();
	}

	void EditorRuntimePipeline::finalize()
	{
	}

	void EditorRuntimePipeline::run()
	{
		MEMORY_GUARD;

		// creating editor winâow
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
} // namespace engine