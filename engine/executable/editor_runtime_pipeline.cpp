#include "editor_runtime_pipeline.h"

#include <iostream>

#include <engine/core/time/time_manager.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/window/window_manager.h>

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	EditorRuntimePipeline::EditorRuntimePipeline()
		: m_editorWindow(nullptr)
	{}

	bool EditorRuntimePipeline::initialize()
	{
		MEMORY_GUARD;

		bool result = true;

#if USE_LOGGER
		result &= initializeLogger();
#endif // #if USE_LOGGER
		INFO_LOG("Initialization started.");

		result &= initializeGLFW();
		result &= initializeWindowManager();
		result &= initializeGraphicAPI();
		result &= initializeEditor();
		result &= initializeTimeManager();
		result &= initializeObjectsManager();
		result &= initializeComponentsManager();

		INFO_LOG("Initialization done.");

		return result;
	}

	void EditorRuntimePipeline::finalize()
	{
		DEBUG_LOG("Finalization started.");

		finalizeGraphicAPI();
		finalizeGLFW();
	}

	void EditorRuntimePipeline::run()
	{
		MEMORY_GUARD;

		INFO_LOG("Runtime started.");

		m_isRunning = true;
		while (isRunning())
		{
			ScopedTime timer = startDeltaTimer();

			glfwPollEvents();
			m_windowManager->swapBuffers(m_editorWindow);
		}
	}

	bool EditorRuntimePipeline::initializeEditor()
	{
		MEMORY_GUARD;

		bool result = true;

		DEBUG_LOG("Initializing editor...");
		result &= createEditorWindow();

		return result;
	}

	bool EditorRuntimePipeline::createEditorWindow()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Creating editor window...");
		m_editorWindow = m_windowManager->createWindow(400, 400, "Editor");
		if (!m_editorWindow)
			return false;

		m_windowManager->setWindowAsCurrentContext(m_editorWindow);
		return true;
	}
} // namespace engine