#include "editor.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager_accessor.h>
#include <engine/core/entity/entity_manager_impl.h>
#include <engine/core/input/input_system_accessor.h>
#include <engine/core/math/math_helpers.h>
#include <engine/core/resources/resource_manager.h>

#include <engine/debug/memory/memory_guard.h>

#include <engine/editor/editor_selection.h>
#include <engine/editor/entity_selection.h>
#include <engine/editor/gizmo/gizmo_manager.h>
#include <engine/editor/gui/editor_viewport_window.h>
#include <engine/editor/gui/imgui_scoped_frame.h>
#include <engine/editor/gui/imgui_scoped_guards.h>
#include <engine/editor/viewport/editor_framebuffer.h>

#include <engine/internal/helpers/assert.h>
#include <engine/internal/render/glad/glad_editor_drawer.h>
#include <engine/internal/render/i_render_pipeline.h>

#include <engine/render/window/window_manager.h>

namespace engine
{
	constexpr Vector2 DEFAULT_CAMERA_SENSITIVITY = Vector2(1.0f, 1.0f);
	constexpr float DEFAULT_CAMERA_SPEED = 0.05f;

	Editor::Editor(WindowID editorWindow,
				   std::shared_ptr<WindowManager> windowManager,
				   std::shared_ptr<IRenderPipeline> renderPipeline,
				   std::shared_ptr<InputSystemAccessor> inputSystemAccessor,
				   EntityManagerAccessor* entityManagerAccessor,
				   std::shared_ptr<ResourceManager> resourceManager)
		// dependencies
		: m_editorWindow(editorWindow)
		, m_windowManager(std::move(windowManager))
		, m_renderPipeline(std::move(renderPipeline))
		, m_inputSystemAccessor(std::move(inputSystemAccessor))
		, m_entityManagerAccessor(entityManagerAccessor)
		, m_resourceManager(resourceManager)
		// default settings
		, m_editorCameraMovementSpeed(DEFAULT_CAMERA_SPEED)
		, m_editorCameraSensitivity(DEFAULT_CAMERA_SENSITIVITY)
	{
		DEBUG_ASSERT(m_editorWindow != WindowID());
		DEBUG_ASSERT(m_windowManager != nullptr);
		DEBUG_ASSERT(m_renderPipeline != nullptr);
		DEBUG_ASSERT(m_inputSystemAccessor != nullptr);
	}

	bool Editor::initialize()
	{
		bool result = true;

		result &= initializeViewports();
		result &= initializeGizmoManager();
		result &= initializeSelectionManagers();
		result &= initializeImGui();
		result &= initializeDrawer();

		createViewport();
		createViewport();

		return result;
	}

	void Editor::tick()
	{
		drawEditorUI();

		m_renderPipeline->renderEditorViewports();

		m_editorSelection->handleSelection();
		m_gizmoManager->tick();
	}

	void Editor::drawEditorUI()
	{
		ImGuiScopedFrame imguiFrame;

		drawViewports();
		drawWorldObjectsMenu();
	}

	void Editor::createViewport()
	{
		auto viewportFramebuffer = m_renderPipeline->createFramebuffer(1000, 1000);
		auto selectionFramebuffer = m_renderPipeline->createFramebuffer(1000, 1000);
		m_editorViewports->createViewport(
			{
				std::move(viewportFramebuffer),
				std::move(selectionFramebuffer)
			}, m_inputSystemAccessor->inputSystem());
	}

	void Editor::drawViewports()
	{
		m_editorViewports->onBeforeDraw();
		for (auto& slot : m_editorViewports->viewports())
		{
			if (slot.empty())
				continue;
			auto& viewport = slot.get();

			if (!m_gizmoManager->isGizmoDragging())
			{
				if (viewport.isFocused())
				{
					handleViewportInput(viewport);
				}

				const auto& inputSystem = m_inputSystemAccessor->inputSystem();
				if (viewport.isHovered() && inputSystem->onMouseButtonDown(MouseButton::Right))
				{
					viewport.focus();
				}
			}

			viewport.draw();
		}
	}

	void Editor::handleViewportInput(EditorViewportWindow& viewport)
	{
		bool anyInput = false;

		Vector3 movement = Vector3::zero();

		const auto& inputSystem = m_inputSystemAccessor->inputSystem();
		if (inputSystem->isKeyPressed(KeyCode::W))
		{
			anyInput = true;
			movement.z += 1;
		}
		else if (inputSystem->isKeyPressed(KeyCode::S))
		{
			anyInput = true;
			movement.z -= 1;
		}

		if (inputSystem->isKeyPressed(KeyCode::A))
		{
			anyInput = true;
			movement.x -= 1;
		}
		else if (inputSystem->isKeyPressed(KeyCode::D))
		{
			anyInput = true;
			movement.x += 1;
		}

		if (inputSystem->isKeyPressed(KeyCode::Q))
		{
			anyInput = true;
			movement.y -= 1;
		}
		else if (inputSystem->isKeyPressed(KeyCode::E))
		{
			anyInput = true;
			movement.y += 1;
		}

		Vector2 mouseMotion = Vector3::zero();
		if (inputSystem->isMouseButtonPressed(MouseButton::Right))
		{
			mouseMotion = inputSystem->mouseMotion();
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
		ImGuiScopedStyleVar var(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin("World Objects", nullptr, ImGuiWindowFlags_NoCollapse))
		{
			const auto& names = m_entityManagerAccessor->entityManagerInternal()->entityNames();
			for (const auto& it = names.begin(); it != names.end(); ++it)
			{
				if (it->empty())
					continue;
				const std::string& name = it->get();

				if (ImGui::Selectable((name + "##" + std::to_string(it.getIndex())).c_str()))
				{
					m_entitySelection->deselectAll();
					m_entitySelection->select(EntityID(it.getIndex()));
				}
			}
		}
		ImGui::End();
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

		m_entitySelection = std::shared_ptr<EntitySelection>(
			new EntitySelection({
				m_entityManagerAccessor->entityManager(),
				m_gizmoManager.get()
			}));
		m_editorSelection = std::shared_ptr<EditorSelectionManager>(
			new EditorSelectionManager(
				m_editorViewports,
				m_inputSystemAccessor,
				m_entitySelection,
				m_gizmoManager
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
		m_gizmoManager = std::shared_ptr<GizmoManager>(
			new GizmoManager({
				m_entityManagerAccessor->entityManager(),
				m_inputSystemAccessor->inputSystem().get(),
				this
			}));
		m_gizmoManager->setGizmoType(GizmoType::Translate);
		return m_gizmoManager != nullptr;
	}
} // namespace engine