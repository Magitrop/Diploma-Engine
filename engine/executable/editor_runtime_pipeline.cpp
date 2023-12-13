#include "editor_runtime_pipeline.h"

#include <iostream>

#include <engine/internal/render/graphics/glad/glad_render_component.h>
#include <engine/internal/render/graphics/i_render_pipeline.h>

#include <engine/core/components/component_manager.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/resources/resource_manager.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/time/time_manager.h>
#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/helpers/scoped_sequential_executor.h>
#include <engine/internal/render/window/window_manager.h>

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	EditorRuntimePipeline::EditorRuntimePipeline()
		: m_editorWindow(WindowID())
	{}

	bool EditorRuntimePipeline::initialize()
	{
		MEMORY_GUARD;

		ScopedSequentialExecutor executor(this);

#if USE_LOGGER
		executor(&EditorRuntimePipeline::initializeLogger);
#endif // #if USE_LOGGER
		INFO_LOG("Initialization started.");

		executor(&EditorRuntimePipeline::initializeGLFW);
		executor(&EditorRuntimePipeline::initializeWindowManager);
		executor(&EditorRuntimePipeline::initializeEditor);
		executor(&EditorRuntimePipeline::initializeGraphicAPI);
		executor(&EditorRuntimePipeline::initializeTimeManager);
		executor(&EditorRuntimePipeline::initializeComponentRegistrar);
		executor(&EditorRuntimePipeline::initializeEntityManager);

		executor(&EditorRuntimePipeline::registerBuiltinComponents);

		executor(&EditorRuntimePipeline::initializeResourceManager);
		executor(&EditorRuntimePipeline::initializeRenderPipeline);

		executor(&EditorRuntimePipeline::registerBuiltinResources);

		if (executor.result())
			INFO_LOG("Initialization done.");
		else
			ERROR_LOG("Initialization failed!");

		return executor.result();
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

		auto manager = m_entityManager->getComponentManager<RenderComponent>();
		auto entity = m_entityManager->createEntity();
		auto componentID = m_entityManager->attachComponent<RenderComponent>(entity);
		auto materialID = m_resourceManager->findMaterial("default");
		manager->setMaterial(componentID, materialID);

		m_isRunning = true;
		while (isRunning())
		{
			ScopedTime timer = startDeltaTimer();

			m_renderPipeline->renderFrame();

			glfwPollEvents();
			m_windowManager->swapBuffers(m_editorWindow);
		}
	}

	bool EditorRuntimePipeline::initializeEditor()
	{
		MEMORY_GUARD;

		bool result = true;

		DEBUG_LOG("Initializing editor...");

		ScopedSequentialExecutor executor(this);

		executor(&EditorRuntimePipeline::createEditorWindow);

		return executor.result();
	}

	bool EditorRuntimePipeline::createEditorWindow()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Creating editor window...");
		m_editorWindow = m_windowManager->createWindow(400, 400, "Editor");
		if (!m_editorWindow.isValid())
		{
			FATAL_LOG("Failed to create Editor window.");
			return false;
		}

		m_windowManager->setWindowAsCurrentContext(m_editorWindow);
		return true;
	}
} // namespace engine