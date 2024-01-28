#pragma once

#include <memory>

#include <engine/core/math/vector2.h>
#include <engine/render/window/window.h>

namespace engine
{
	class EditorDrawer;
	class EditorViewportsManager;
	class EditorViewportWindow;
	class EntityManager;
	class EntitySelection;
	class InputSystem;
	class IRenderPipeline;
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
		std::shared_ptr<EditorDrawer> drawer();

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
		[[nodiscard]] bool initializeSelectionManager();
		[[nodiscard]] bool initializeDrawer();

		WindowID m_editorWindow;
		std::shared_ptr<EditorViewportsManager> m_editorViewports;
		std::shared_ptr<WindowManager> m_windowManager;
		std::shared_ptr<InputSystem> m_inputSystem;
		std::shared_ptr<IRenderPipeline> m_renderPipeline;
		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<EntitySelection> m_entitySelection;
		std::shared_ptr<EditorDrawer> m_editorDrawer;
		std::shared_ptr<ResourceManager> m_resourceManager;

		float m_editorCameraMovementSpeed;
		Vector2 m_editorCameraSensitivity;
	};
} // namespace engine