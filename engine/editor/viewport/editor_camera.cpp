#include "editor_camera.h"

#if IS_EDITOR
namespace engine
{
	constexpr Vector3 DEFAULT_EYE_POSITION = Vector3(0, 0, 0);
	constexpr Vector3 DEFAULT_EYE_FORWARD = Vector3(0, 0, 0);
	constexpr float DEFAULT_FOV = 60.0f;
	constexpr float DEFAULT_NEAR_CLIP_PLANE = 0.1f;
	constexpr float DEFAULT_FAR_CLIP_PLANE = 100.0f;

	EditorCamera EditorViewports::createCamera(std::shared_ptr<IFramebuffer> framebuffer)
	{
		return
		{
			m_eyePosition.push(DEFAULT_EYE_POSITION)->get(),
			m_eyeForward.push(DEFAULT_EYE_FORWARD)->get(),
			m_fov.push(DEFAULT_FOV)->get(),
			m_nearClipPlane.push(DEFAULT_NEAR_CLIP_PLANE)->get(),
			m_farClipPlane.push(DEFAULT_FAR_CLIP_PLANE)->get(),
			m_framebuffer.push(std::move(framebuffer))->get()
		};
	}

	EditorCameraIterator EditorViewports::begin()
	{
		return
		{
			m_eyePosition.begin(),
			m_eyeForward.begin(),
			m_fov.begin(),
			m_nearClipPlane.begin(),
			m_farClipPlane.begin(),
			m_framebuffer.begin()
		};
	}

	EditorCameraIterator EditorViewports::end()
	{
		return
		{
			m_eyePosition.end(),
			m_eyeForward.end(),
			m_fov.end(),
			m_nearClipPlane.end(),
			m_farClipPlane.end(),
			m_framebuffer.end()
		};
	}

	EditorCameraIterator& EditorCameraIterator::operator ++ ()
	{
		++cameraPositionIt;
		++cameraForwardIt;
		++fovIt;
		++nearClipPlaneIt;
		++farClipPlaneIt;
		++framebufferIt;
		return *this;
	}

	bool EditorCameraIterator::operator!=(const EditorCameraIterator& other) const
	{
		bool result = true;

		result &= cameraPositionIt	!= other.cameraPositionIt;
		result &= cameraForwardIt	!= other.cameraForwardIt;
		result &= fovIt				!= other.fovIt;
		result &= nearClipPlaneIt	!= other.nearClipPlaneIt;
		result &= farClipPlaneIt	!= other.farClipPlaneIt;
		result &= framebufferIt		!= other.framebufferIt;

		return result;
	}

	bool EditorCameraIterator::operator == (const EditorCameraIterator& other) const
	{
		return !operator != (other);
	}
	EditorCamera EditorCameraIterator::operator * ()
	{
		return
		{
			cameraPositionIt->get(),
			cameraForwardIt->get(),
			fovIt->get(),
			nearClipPlaneIt->get(),
			farClipPlaneIt->get(),
			framebufferIt->get(),
		};
	}
} // namespace engine
#endif // #if IS_EDITOR