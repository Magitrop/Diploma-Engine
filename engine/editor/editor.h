#pragma once

#include <memory>

#include <engine/core/math/vector2.h>
#include <engine/render/window/window.h>

namespace engine
{
	class IEditorDrawer;
	class EditorViewportsManager;
	class EditorViewportWindow;
	class EditorSelectionManager;
	class EntityManager;
	class EntitySelection;
	class InputSystem;
	class IRenderPipeline;
	class GizmoManager;
	class ResourceManager;
	class WindowManager;
	class Editor final
	{
	public:
		explicit Editor(WindowID editorWindow,
						std::shared_ptr<WindowManager> windowManager,
						std::shared_ptr<IRenderPipeline> renderPipeline,
						std::shared_ptr<InputSystem> inputSystem,
						std::shared_ptr<EntityManager> entityManager,
						std::shared_ptr<ResourceManager> resourceManager);

		bool initialize();

		void tick();

		void createViewport();

		WindowID getEditorWindow() const;

		std::shared_ptr<EntitySelection> entitySelection();
		std::shared_ptr<EditorViewportsManager> viewportsManager();
		std::shared_ptr<IEditorDrawer> drawer();

	private:
		void drawEditorUI();
		void drawGizmos();

		// Draws all Viewport windows.
		void drawViewports();
		void handleViewportInput(EditorViewportWindow& viewport);

		// Draws the 'World Objects' menu window.
		void drawWorldObjectsMenu();

		[[nodiscard]] bool initializeImGui();
		[[nodiscard]] bool initializeViewports();
		[[nodiscard]] bool initializeSelectionManagers();
		[[nodiscard]] bool initializeDrawer();
		[[nodiscard]] bool initializeGizmoManager();

		WindowID m_editorWindow;

		std::shared_ptr<EditorViewportsManager> m_editorViewports;
		std::shared_ptr<WindowManager> m_windowManager;
		std::shared_ptr<InputSystem> m_inputSystem;
		std::shared_ptr<IRenderPipeline> m_renderPipeline;
		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<EntitySelection> m_entitySelection;
		std::shared_ptr<EditorSelectionManager> m_editorSelection;
		std::shared_ptr<IEditorDrawer> m_editorDrawer;
		std::shared_ptr<ResourceManager> m_resourceManager;
		std::shared_ptr<GizmoManager> m_gizmoManager;

		float m_editorCameraMovementSpeed;
		Vector2 m_editorCameraSensitivity;
	};
} // namespace engine