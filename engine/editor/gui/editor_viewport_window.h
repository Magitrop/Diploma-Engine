#pragma once

#include <memory>

#include <engine/dependencies/imgui/imgui.h>
#include <engine/editor/viewport/editor_camera.h>

namespace engine
{
	class InputSystem;
	class EditorViewportWindow final
	{
	private:
		friend class EditorViewports;

	private:
		explicit EditorViewportWindow(std::shared_ptr<InputSystem> inputSystem,
									  EditorCamera camera,
									  std::size_t index);

	public:
		//ImVec2 windowSize() const;

	private:
		void draw() const;

		std::shared_ptr<InputSystem> m_inputSystem;
		EditorCamera m_editorCamera;
		//ImVec2 m_windowSize; // It is actually ImGui::GetContentRegionAvail()
		std::size_t m_viewportIndex;
	};

	struct EditorCameraIterator;
	// A manager for all editor cameras.
	class EditorViewports final
	{
		// friends
	private:
		friend class IRenderPipeline;
		friend struct EditorCameraIterator;

		// members
	public:
		explicit EditorViewports() = default;

		// Draws all viewport ImGui windows.
		void draw() const;

		const EditorCameraIterator begin() const;
		const EditorCameraIterator end() const;

	private:
		EditorViewportWindow createViewport(std::shared_ptr<IFramebuffer> framebuffer,
											std::shared_ptr<InputSystem> inputSystem);

		PersistentVector<Vector3, constants::RENDER_CONTEXTS_PER_PAGE> m_eyePosition;
		PersistentVector<Vector3, constants::RENDER_CONTEXTS_PER_PAGE> m_eyeAngles;
		PersistentVector<float, constants::RENDER_CONTEXTS_PER_PAGE> m_fov;
		PersistentVector<float, constants::RENDER_CONTEXTS_PER_PAGE> m_nearClipPlane;
		PersistentVector<float, constants::RENDER_CONTEXTS_PER_PAGE> m_farClipPlane;
		PersistentVector<std::shared_ptr<IFramebuffer>, constants::RENDER_CONTEXTS_PER_PAGE> m_framebuffer;
		PersistentVector<std::shared_ptr<EditorViewportWindow>, constants::RENDER_CONTEXTS_PER_PAGE> m_viewports;
	};

	struct EditorCameraIterator
	{
		decltype(EditorViewports::m_eyePosition)::Iterator		cameraPositionIt;
		decltype(EditorViewports::m_eyeAngles)::Iterator		cameraAnglesIt;
		decltype(EditorViewports::m_fov)::Iterator				fovIt;
		decltype(EditorViewports::m_nearClipPlane)::Iterator	nearClipPlaneIt;
		decltype(EditorViewports::m_farClipPlane)::Iterator		farClipPlaneIt;
		decltype(EditorViewports::m_framebuffer)::Iterator		framebufferIt;

		EditorCamera operator * ();
		EditorCameraIterator& operator ++();
		bool operator != (const EditorCameraIterator& other) const;
		bool operator == (const EditorCameraIterator& other) const;
	};
} // namespace engine
