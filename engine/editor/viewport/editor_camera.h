#pragma once

#if IS_EDITOR
#include <engine/core/math/vector3.h>
#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/non_copyable.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class IFramebuffer;
	struct EditorCamera final
	{
		Vector3& cameraPosition;
		Vector3& cameraForward;
		float& fov;
		float& nearClipPlane;
		float& farClipPlane;
		std::shared_ptr<IFramebuffer>& framebuffer;
	};

	class EditorCameraIterator;
	// A manager for all editor cameras.
	class EditorViewports final
	{
		// friends
	private:
		friend class IRenderPipeline;
		friend class EditorCameraIterator;
		
		// members
	public:
		explicit EditorViewports() = default;

		EditorCameraIterator begin();
		EditorCameraIterator end();

	private:
		EditorCamera createCamera(std::shared_ptr<IFramebuffer> framebuffer);

		PersistentVector<Vector3, constants::RENDER_CONTEXTS_PER_PAGE> m_eyePosition;
		PersistentVector<Vector3, constants::RENDER_CONTEXTS_PER_PAGE> m_eyeForward;
		PersistentVector<float, constants::RENDER_CONTEXTS_PER_PAGE> m_fov;
		PersistentVector<float, constants::RENDER_CONTEXTS_PER_PAGE> m_nearClipPlane;
		PersistentVector<float, constants::RENDER_CONTEXTS_PER_PAGE> m_farClipPlane;
		PersistentVector<std::shared_ptr<IFramebuffer>, constants::RENDER_CONTEXTS_PER_PAGE> m_framebuffer;
	};

	struct EditorCameraIterator
	{
		decltype(EditorViewports::m_eyePosition)::Iterator		cameraPositionIt;
		decltype(EditorViewports::m_eyeForward)::Iterator		cameraForwardIt;
		decltype(EditorViewports::m_fov)::Iterator				fovIt;
		decltype(EditorViewports::m_nearClipPlane)::Iterator	nearClipPlaneIt;
		decltype(EditorViewports::m_farClipPlane)::Iterator		farClipPlaneIt;
		decltype(EditorViewports::m_framebuffer)::Iterator		framebufferIt;

		EditorCameraIterator& operator ++();
		bool operator != (const EditorCameraIterator& other) const;
		bool operator == (const EditorCameraIterator& other) const;
		EditorCamera operator * ();
	};
} // namespace engine
#endif // #if IS_EDITOR