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
	class EntityManagerAccessor;
	class EntitySelection;
	class InputSystemAccessor;
	class IRenderPipeline;
	class GizmoManager;
	class ResourceManager;
	class WindowManager;
	class Editor final
	{
		// friends
	private:
		friend class EditorRuntimePipeline;

		// members
	public:
		explicit Editor(WindowID editorWindow,
						std::shared_ptr<WindowManager> windowManager,
						std::shared_ptr<IRenderPipeline> renderPipeline,
						std::shared_ptr<InputSystemAccessor> inputSystemAccessor,
						EntityManagerAccessor* entityManagerAccessor,
						std::shared_ptr<ResourceManager> resourceManager);

		WindowID getEditorWindow() const { return m_editorWindow; }

		std::shared_ptr<EntitySelection> entitySelection() { return m_entitySelection; }
		std::shared_ptr<EditorViewportsManager> viewportsManager() { return m_editorViewports; }
		std::shared_ptr<IEditorDrawer> drawer() { return m_editorDrawer; }
		std::shared_ptr<ResourceManager> resourceManager() { return m_resourceManager; }

	private:
		bool initialize();

		void tick();
		void drawEditorUI();

		// Draws all Viewport windows.
		void drawViewports();
		void handleViewportInput(EditorViewportWindow& viewport);
		void createViewport();

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
		std::shared_ptr<InputSystemAccessor> m_inputSystemAccessor;
		std::shared_ptr<IRenderPipeline> m_renderPipeline;
		EntityManagerAccessor* m_entityManagerAccessor;
		std::shared_ptr<EntitySelection> m_entitySelection;
		std::shared_ptr<EditorSelectionManager> m_editorSelection;
		std::shared_ptr<IEditorDrawer> m_editorDrawer;
		std::shared_ptr<ResourceManager> m_resourceManager;
		std::shared_ptr<GizmoManager> m_gizmoManager;

		float m_editorCameraMovementSpeed;
		Vector2 m_editorCameraSensitivity;
	};
} // namespace engine