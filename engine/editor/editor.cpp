#include "editor.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/input/input_system.h>
#include <engine/core/resources/resource_manager.h>

#include <engine/debug/memory/memory_guard.h>

#include <engine/editor/editor_selection.h>
#include <engine/editor/entity_selection.h>
#include <engine/editor/gizmo/gizmo_manager.h>
#include <engine/editor/gui/editor_viewport_window.h>
#include <engine/editor/gui/imgui_scoped_frame.h>
#include <engine/editor/viewport/editor_framebuffer.h>

#include <engine/internal/helpers/assert.h>
#include <engine/internal/render/glad/glad_editor_drawer.h>
#include <engine/internal/render/i_render_pipeline.h>

#include <engine/render/window/window_manager.h>

#include <engine/dependencies/imguizmo/ImGuizmo.h>

namespace engine
{
	constexpr Vector2 DEFAULT_CAMERA_SENSITIVITY = Vector2(1.0f, 1.0f);
	constexpr float DEFAULT_CAMERA_SPEED = 0.05f;

	Editor::Editor(WindowID editorWindow,
				   std::shared_ptr<WindowManager> windowManager,
				   std::shared_ptr<IRenderPipeline> renderPipeline,
				   std::shared_ptr<InputSystem> inputSystem,
				   std::shared_ptr<EntityManager> entityManager,
				   std::shared_ptr<ResourceManager> resourceManager)
		// dependencies
		: m_editorWindow(editorWindow)
		, m_windowManager(std::move(windowManager))
		, m_renderPipeline(std::move(renderPipeline))
		, m_inputSystem(inputSystem)
		, m_entityManager(entityManager)
		, m_resourceManager(resourceManager)
		// default settings
		, m_editorCameraMovementSpeed(DEFAULT_CAMERA_SPEED)
		, m_editorCameraSensitivity(DEFAULT_CAMERA_SENSITIVITY)
	{
		DEBUG_ASSERT(m_editorWindow != WindowID());
		DEBUG_ASSERT(m_windowManager != nullptr);
		DEBUG_ASSERT(m_renderPipeline != nullptr);
		DEBUG_ASSERT(m_inputSystem != nullptr);
	}

	bool Editor::initialize()
	{
		bool result = true;

		result &= initializeViewports();
		result &= initializeSelectionManagers();
		result &= initializeImGui();
		result &= initializeGizmoManager();
		result &= initializeDrawer();

		createViewport();

		return result;
	}

	void Editor::tick()
	{
		drawEditorUI();

		m_renderPipeline->renderEditorViewports();

		auto pixel = m_editorSelection->handleSelection();
		DEBUG_LOG("{} {} {} {} {}", pixel.color[0], pixel.color[1], pixel.color[2], pixel.color[3], pixel.depth);
	}

	void Editor::drawEditorUI()
	{
		ImGuiScopedFrame imguiFrame;

		drawGizmos();
		drawViewports();
	}

	void Editor::drawGizmos()
	{
		/*auto transformManager = m_entityManager->getComponentManager<Transform>();
		for (const auto& selected : m_entitySelection->selection())
		{
			ComponentID transform = m_entityManager->getComponent<Transform>(selected);
			Matrix4x4& model = transformManager->modelMatrix(transform);
			for (auto& window : m_editorViewports->viewports())
			{
				auto& viewport = window.get().viewport();
			}
		}*/
	}

	void Editor::createViewport()
	{
		auto viewportFramebuffer = m_renderPipeline->createFramebuffer(400, 400);
		auto selectionFramebuffer = m_renderPipeline->createFramebuffer(400, 400);
		m_editorViewports->createViewport(
			{
				std::move(viewportFramebuffer),
				std::move(selectionFramebuffer)
			}, m_inputSystem);
	}

	WindowID Editor::getEditorWindow() const
	{
		return m_editorWindow;
	}

	std::shared_ptr<EntitySelection> Editor::entitySelection()
	{
		return m_entitySelection;
	}

	std::shared_ptr<EditorViewportsManager> Editor::viewportsManager()
	{
		return m_editorViewports;
	}

	std::shared_ptr<IEditorDrawer> Editor::drawer()
	{
		return m_editorDrawer;
	}

	void Editor::drawViewports()
	{
		m_editorViewports->onBeforeDraw();
		for (auto& slot : m_editorViewports->viewports())
		{
			if (slot.empty())
				continue;
			auto& viewport = slot.get();

			if (viewport.isFocused())
			{
				handleViewportInput(viewport);
			}

			if (viewport.isHovered() && m_inputSystem->onMouseButtonDown(MouseButton::Right))
			{
				viewport.focus();
			}

			viewport.draw();
		}
	}

	void Editor::handleViewportInput(EditorViewportWindow& viewport)
	{
		bool anyInput = false;

		Vector3 movement = Vector3::zero();

		if (m_inputSystem->isKeyPressed(KeyCode::W))
		{
			anyInput = true;
			movement.z += 1;
		}
		else if (m_inputSystem->isKeyPressed(KeyCode::S))
		{
			anyInput = true;
			movement.z -= 1;
		}

		if (m_inputSystem->isKeyPressed(KeyCode::A))
		{
			anyInput = true;
			movement.x -= 1;
		}
		else if (m_inputSystem->isKeyPressed(KeyCode::D))
		{
			anyInput = true;
			movement.x += 1;
		}

		if (m_inputSystem->isKeyPressed(KeyCode::Q))
		{
			anyInput = true;
			movement.y -= 1;
		}
		else if (m_inputSystem->isKeyPressed(KeyCode::E))
		{
			anyInput = true;
			movement.y += 1;
		}

		Vector2 mouseMotion = Vector3::zero();
		if (m_inputSystem->isMouseButtonPressed(MouseButton::Right))
		{
			mouseMotion = m_inputSystem->mouseMotion();
			anyInput = true;
		}

		if (anyInput)
		{
			viewport.handleCameraInput(
				mouseMotion, movement,
				m_editorCameraMovementSpeed,
				m_editorCameraSensitivity
			);
		}
	}

	void Editor::drawWorldObjectsMenu()
	{
	}

	bool Editor::initializeImGui()
	{
		ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
		return true;
	}

	bool Editor::initializeViewports()
	{
		MEMORY_GUARD;

		m_editorViewports = std::shared_ptr<EditorViewportsManager>(new EditorViewportsManager());
		return m_editorViewports != nullptr;
	}

	bool Editor::initializeSelectionManagers()
	{
		MEMORY_GUARD;

		m_entitySelection = std::shared_ptr<EntitySelection>(new EntitySelection());
		m_editorSelection = std::shared_ptr<EditorSelectionManager>(
			new EditorSelectionManager(
				m_editorViewports,
				m_inputSystem,
				m_entitySelection
			));
		return m_entitySelection != nullptr && m_editorSelection != nullptr;
	}

	bool Editor::initializeDrawer()
	{
		MEMORY_GUARD;

		// TODO: abstract render pipeline
		m_editorDrawer = std::shared_ptr<IEditorDrawer>(
			new GladEditorDrawer(
				m_renderPipeline,
				m_resourceManager,
				m_gizmoManager
			));
		return m_editorDrawer != nullptr;
	}

	bool Editor::initializeGizmoManager()
	{
		MEMORY_GUARD;

		// TODO: abstract render pipeline
		m_gizmoManager = std::shared_ptr<GizmoManager>(new GizmoManager(m_entityManager));
		return m_gizmoManager != nullptr;
	}
} // namespace engine