#include "editor_runtime_pipeline.h"

#include <iostream>

#include <engine/internal/render/glad/glad_mesh_renderer_component.h>
#include <engine/internal/render/i_render_pipeline.h>

#include <engine/core/components/component_manager.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/resources/resource_manager.h>
#include <engine/core/time/time_manager.h>

#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>

#include <engine/editor/gui/imgui_scoped_frame.h>
#include <engine/editor/viewport/editor_framebuffer.h>

#include <engine/internal/helpers/scoped_sequential_executor.h>

#include <engine/render/window/window_manager_impl.h>

#include <engine/dependencies/glfw/include/GLFW/glfw3.h>
#include <engine/dependencies/imgui/backends/imgui_impl_opengl3.h>
#include <engine/dependencies/imgui/backends/imgui_impl_glfw.h>

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
		executor(&EditorRuntimePipeline::initializeInputSystem);
		executor(&EditorRuntimePipeline::initializeWindowManager);
		executor(&EditorRuntimePipeline::initializeEditor);
		executor(&EditorRuntimePipeline::initializeGraphicAPI);
		executor(&EditorRuntimePipeline::initializeTimeManager);
		executor(&EditorRuntimePipeline::initializeComponentRegistrar);
		executor(&EditorRuntimePipeline::initializeEntityManager);
		executor(&EditorRuntimePipeline::initializeResourceManager);

		executor(&EditorRuntimePipeline::registerBuiltinComponents);
		executor(&EditorRuntimePipeline::registerBuiltinResources);

		executor(&EditorRuntimePipeline::initializeRenderPipeline);

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

		auto renderer = m_entityManager->getComponentManager<MeshRenderer>();
		auto entity = m_entityManager->createEntity();
		auto rendererID = m_entityManager->attachComponent<MeshRenderer>(entity);
		auto materialID = m_resourceManager->findMaterial("default");

		auto meshID1 = m_resourceManager->createMesh();
		std::vector<Vertex> v1;
		v1.emplace_back(Vector3(-1, -1, 0));
		v1.emplace_back(Vector3(1, -1, 0));
		v1.emplace_back(Vector3(0, 1, 0));
		m_resourceManager->getMeshByID(meshID1)->setVertices(std::move(v1));

		auto meshID2 = m_resourceManager->createMesh();
		std::vector<Vertex> v2;
		v2.emplace_back(Vector3(-1, -1, 0));
		v2.emplace_back(Vector3(0, -1, 0));
		v2.emplace_back(Vector3(0, 1, 0));
		m_resourceManager->getMeshByID(meshID2)->setVertices(std::move(v2));

		auto meshID3 = m_resourceManager->createMesh();
		std::vector<Vertex> v3;
		v3.emplace_back(Vector3(-1, -1, 0));
		v3.emplace_back(Vector3(1, 0, 0));
		v3.emplace_back(Vector3(0, 1, 0));
		m_resourceManager->getMeshByID(meshID3)->setVertices(std::move(v3));

		//renderer->setMaterial(rendererID, materialID);
		renderer->setMesh(rendererID, meshID1);
		renderer->setMesh(rendererID, meshID2);
		renderer->setMesh(rendererID, meshID3);

		m_isRunning = true;
		float x = 0;

		while (isRunning())
		{
			ScopedTime timer = startDeltaTimer();
			m_renderPipeline->renderEditorViewports();

			// editor->draw
			{
				ImGuiScopedFrame imguiFrame;

				std::size_t viewportIndex = 0;
				EditorViewports viewports = *m_editorViewports.get();
				for (const auto& camera : viewports)
				{
					std::string viewportName = std::format("Viewport {}", ++viewportIndex);

					EditorFramebuffer* framebuffer = static_cast<EditorFramebuffer*>(camera.framebuffer.get());
					if (ImGui::Begin(viewportName.c_str(), nullptr, ImGuiWindowFlags_MenuBar))
					{
						ImVec2 size = ImVec2(camera.framebuffer->width(), camera.framebuffer->height());
						ImGui::Image(framebuffer->textureID(), size);
						ImGui::End();
					}
				}
			}
			glfwPollEvents();

			std::vector<Vertex> v4;
			v4.emplace_back(Vector3(-1, 0, 0));
			v4.emplace_back(Vector3(x, -1, 0));
			v4.emplace_back(Vector3(0, 1, 0));
			m_resourceManager->getMeshByID(meshID3)->setVertices(std::move(v4));
			x = sin(m_timeManager->timeSinceLaunch());

			m_windowManager->m_internal->swapBuffers(m_editorWindow);
		}
	}

	bool EditorRuntimePipeline::initializeEditor()
	{
		MEMORY_GUARD;

		bool result = true;

		DEBUG_LOG("Initializing editor...");

		ScopedSequentialExecutor executor(this);

		executor(&EditorRuntimePipeline::createEditorWindow);
		executor(&EditorRuntimePipeline::initializeImGui);
		executor(&EditorRuntimePipeline::initializeViewports);

		return executor.result();
	}

	bool EditorRuntimePipeline::initializeViewports()
	{
		MEMORY_GUARD;

		m_editorViewports = std::shared_ptr<EditorViewports>(new EditorViewports());
		return m_editorViewports != nullptr;
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
		result &= m_renderPipeline->initialize(m_editorViewports);

		createEditorViewport(400, 400);
		createEditorViewport(400, 400);
		return result;
	}

	EditorCamera EditorRuntimePipeline::createEditorViewport(std::size_t initialViewportWidth,
															 std::size_t initialViewportHeight)
	{
		return m_renderPipeline->createEditorCamera(initialViewportWidth, initialViewportHeight);
	}

	void EditorRuntimePipeline::finalizeImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}
} // namespace engine