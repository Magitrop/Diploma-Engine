#pragma once

#include <memory>

#include <engine/core/math/vector2.h>
#include <engine/core/math/matrix4x4.h>

#include <engine/dependencies/imgui/imgui.h>

#include <engine/internal/helpers/iterator_provider.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/internal/core/constants/editor_constants.h>

#include <engine/editor/viewport/editor_viewport.h>

namespace engine
{
	class IFramebuffer;
	class InputSystem;
	class EditorViewportWindow final
	{
		// friends
	private:
		friend class Editor;
		friend class EditorViewportsManager;

		// members
	public:
		struct WindowFramebuffers
		{
			std::shared_ptr<IFramebuffer> viewportFramebuffer;
			std::shared_ptr<IFramebuffer> selectionFramebuffer;
		};

	private:
		explicit EditorViewportWindow(WindowFramebuffers framebuffers,
									  EditorViewportsManager* viewportsManager,
									  std::size_t viewportIndex);
	public:
		// Creates an invalid viewport window.
		explicit EditorViewportWindow();
		//ImVec2 windowSize() const;
		bool isHovered() const;
		bool isFocused() const;

		// Makes this ImGui viewport window focused.
		void focus();

		WindowFramebuffers& framebuffers() { return m_windowFramebuffers; }
		const WindowFramebuffers& framebuffers() const { return m_windowFramebuffers; }

		EditorViewport& viewport() { return m_viewport; }
		const EditorViewport& viewport() const { return m_viewport; }

		struct WindowProperties final
		{
			Vector2 position;
			Vector2 topLeft;
			Vector2 bottomRight;
		};
		// Returns the ImGui window properties.
		WindowProperties& properties();
		const WindowProperties& properties() const;

		// Maps global screen coordinates to local window coordinates.
		// Note: content local coordinates exclude ImGui window margins and titlebar.
		Vector2 screenToContent(Vector2 screenCoord) const;

	private:
		void draw();
		void handleCameraInput(Vector2 mouseMotion, Vector3 movementInput,
							   float movementSpeed, Vector2 sensitivity);

		WindowFramebuffers m_windowFramebuffers;

		std::size_t m_viewportIndex;
		EditorViewportsManager* m_viewportsManager;

		bool m_isHovered;
		bool m_isFocused;
		bool m_wantFocus = false;

		EditorViewport m_viewport;
		WindowProperties m_imguiWindowProperties;
	};

	struct EditorCameraIterator;
	// A manager for all editor cameras.
	class EditorViewportsManager final
	{
		// friends
	private:
		friend class Editor;
		friend class EditorViewportWindow;
		friend struct EditorCameraIterator;
		friend struct ViewportIterator;

		// members
	public:
		using Viewports = PersistentVector<EditorViewportWindow, constants::VIEWPORTS_PER_PAGE>;
		explicit EditorViewportsManager() = default;

		const Viewports& viewports();
		const EditorViewportWindow* focusedViewport() const;
		const EditorViewportWindow* hoveredViewport() const;

	private:
		// Called by the Editor every frame before any viewport is drawn.
		void onBeforeDraw();

		EditorViewportWindow& createViewport(EditorViewportWindow::WindowFramebuffers framebuffers,
											 std::shared_ptr<InputSystem> inputSystem);

		Viewports m_viewports;
		EditorViewportWindow* m_focusedViewport = nullptr;
		EditorViewportWindow* m_hoveredViewport = nullptr;
	};
} // namespace engine
