#include "editor_runtime_pipeline.h"

#include <iostream>

#include <engine/internal/render/glad/glad_mesh_renderer_component.h>
#include <engine/internal/render/i_render_pipeline.h>

#include <engine/core/components/component_manager.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/input/input_system_accessor.h>
#include <engine/core/input/input_system_impl.h>
#include <engine/core/math/quaternion.h>
#include <engine/core/math/vector4.h>
#include <engine/core/resources/resource_manager.h>
#include <engine/core/time/time_manager.h>

#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>

#include <engine/editor/entity_selection.h>
#include <engine/editor/gizmo/editor_drawer.h>
#include <engine/editor/gui/imgui_scoped_frame.h>
#include <engine/editor/viewport/editor_framebuffer.h>

#include <engine/internal/helpers/scoped_sequential_executor.h>

#include <engine/render/window/window_manager_impl.h>

#include <engine/dependencies/glfw/include/GLFW/glfw3.h>
#include <engine/dependencies/imgui/backends/imgui_impl_glfw.h>
#include <engine/dependencies/imgui/backends/imgui_impl_opengl3.h>

#include <engine/dependencies/glm/glm/trigonometric.hpp>

namespace engine
{
	EditorRuntimePipeline::EditorRuntimePipeline()
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
		executor(&EditorRuntimePipeline::initializeTimeManager);
		executor(&EditorRuntimePipeline::initializeInputSystem);
		executor(&EditorRuntimePipeline::initializeWindowManager);
		executor(&EditorRuntimePipeline::createEditorWindow);
		executor(&EditorRuntimePipeline::initializeGraphicAPI);
		executor(&EditorRuntimePipeline::initializeComponentRegistrar);
		executor(&EditorRuntimePipeline::initializeEntityManager);
		executor(&EditorRuntimePipeline::initializeResourceManager);

		executor(&EditorRuntimePipeline::registerBuiltinComponents);
		executor(&EditorRuntimePipeline::registerBuiltinResources);

		executor(&EditorRuntimePipeline::initializeRenderPipeline);
		executor(&EditorRuntimePipeline::initializeEditor);

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

		auto materialID = m_resourceManager->findMaterial("default");
		auto renderManager = m_entityManager->getComponentManager<MeshRenderer>();
		auto transformManager = m_entityManager->getComponentManager<Transform>();

		auto entity1 = m_entityManager->createEntity();
		auto entity2 = m_entityManager->createEntity();
		auto entity3 = m_entityManager->createEntity();

		auto rendererID1 = m_entityManager->attachComponent<MeshRenderer>(entity1);
		auto rendererID2 = m_entityManager->attachComponent<MeshRenderer>(entity2);
		auto rendererID3 = m_entityManager->attachComponent<MeshRenderer>(entity3);

		auto transformID1 = m_entityManager->getComponent<Transform>(entity1);
		auto transformID2 = m_entityManager->getComponent<Transform>(entity2);
		auto transformID3 = m_entityManager->getComponent<Transform>(entity3);
		
		auto meshID = m_resourceManager->createMesh();
		std::vector<Vertex> v2
		{
			Vertex(Vector3(0, 0, 0)),
			Vertex(Vector3(0, 1, 0)),
			Vertex(Vector3(0, 0, 1))
		};
		std::vector<std::uint32_t> i2;
		for (int i = 0; i < 3; ++i)
			i2.emplace_back(i);
		m_resourceManager->getMeshByID(meshID)->setVertices(std::move(v2), std::move(i2));

		renderManager->setMaterial(rendererID1, materialID);
		renderManager->setMesh(rendererID1, meshID);

		renderManager->setMaterial(rendererID2, materialID);
		renderManager->setMesh(rendererID2, meshID);

		renderManager->setMaterial(rendererID3, materialID);
		renderManager->setMesh(rendererID3, meshID);

		transformManager->translateLocalBy(transformID1, Vector3(1, 0, 0));
		transformManager->translateLocalBy(transformID2, Vector3(2, 0, 0));
		//transformManager->setParent(transformID1, transformID2);
		//transformManager->setParent(transformID2, transformID3);

		m_isRunning = true;
		while (isRunning())
		{
			ScopedTime timer = startDeltaTimer();

			m_editor->tick();

			m_inputSystemAccessor->onFrameEnd();
			glfwPollEvents();
			m_windowManager->m_internal->swapBuffers(m_editor->getEditorWindow());
		}
	}

	bool EditorRuntimePipeline::createEditorWindow()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Creating editor window...");
		m_editorWindow = m_windowManager->createWindow(1000, 800, "Editor");
		if (!m_editorWindow.isValid())
		{
			FATAL_LOG("Failed to create Editor window.");
			return false;
		}

		m_windowManager->setWindowAsCurrentContext(m_editorWindow);
		return true;
	}

	bool EditorRuntimePipeline::initializeEditor()
	{
		MEMORY_GUARD;

		bool result = true;

		DEBUG_LOG("Initializing editor...");

		DEBUG_ASSERT(m_windowManager != nullptr);
		DEBUG_ASSERT(m_renderPipeline != nullptr);
		DEBUG_ASSERT(m_inputSystem != nullptr);
		DEBUG_ASSERT(m_entityManager != nullptr);
		m_editor = std::shared_ptr<Editor>(new Editor(
			m_editorWindow,
			m_windowManager,
			m_renderPipeline,
			m_inputSystemAccessor,
			m_entityManagerAccessor.get(),
			m_resourceManager
		));

		result &= initializeImGui();
		result &= m_editor->initialize();
		result &= m_renderPipeline->initialize(
			m_editor->viewportsManager(),
			m_editor->drawer()
		);

		return result;
	}

	bool EditorRuntimePipeline::initializeImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();	
		GLFWwindow* window = m_windowManager->m_internal->getCurrentContextGLFWwindow();

		// TODO: abstract graphic API
		if (!ImGui_ImplOpenGL3_Init(nullptr))
		{
			FATAL_LOG("Failed to initialize Dear ImGui.");
			return false;
		}

		// TODO: abstract graphic API
		if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
		{
			FATAL_LOG("Failed to initialize Dear ImGui.");
			return false;
		}

		ImGui::StyleColorsDark();
		return true;
	}

	bool EditorRuntimePipeline::initializeRenderPipeline()
	{
		bool result = true;

		result &= Base::initializeRenderPipeline();
		DEBUG_ASSERT(m_renderPipeline != nullptr);

		return result;
	}

	void EditorRuntimePipeline::finalizeImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}
} // namespace engine