#pragma once

#include <memory>

#include <engine/core/math/vector3.h>
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
	private:
		friend class Editor;
		friend class EditorViewportsManager;

	private:
		explicit EditorViewportWindow(std::shared_ptr<IFramebuffer> framebuffer,
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

		std::shared_ptr<IFramebuffer> framebuffer() const { return m_framebuffer; }

		EditorViewport& viewport();

	private:
		void draw();
		void handleCameraInput(Vector2 mouseMotion, Vector3 movementInput,
							   float movementSpeed, Vector2 sensitivity);
		std::shared_ptr<IFramebuffer> m_framebuffer;

		std::size_t m_viewportIndex;
		EditorViewportsManager* m_viewportsManager;

		bool m_isHovered;
		bool m_isFocused;
		bool m_wantFocus = false;

		EditorViewport m_viewport;
	};

	struct EditorCameraIterator;
	// A manager for all editor cameras.
	class EditorViewportsManager final
	{
		// friends
	private:
		friend class Editor;
		friend struct EditorCameraIterator;
		friend struct ViewportIterator;

		// members
	public:
		using Viewports = PersistentVector<EditorViewportWindow, constants::VIEWPORTS_PER_PAGE>;
		explicit EditorViewportsManager() = default;

		const Viewports& viewports();

	private:
		EditorViewportWindow& createViewport(std::shared_ptr<IFramebuffer> framebuffer,
											 std::shared_ptr<InputSystem> inputSystem);

		Viewports m_viewports;
	};
} // namespace engine
