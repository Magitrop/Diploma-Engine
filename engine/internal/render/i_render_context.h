#pragma once

#include <memory>

#include <engine/core/math/vector3.h>

namespace engine
{
	class IFramebuffer {};

	struct CameraData
	{
		Vector3 position;
		Vector3 forward;
		float fov;
		float nearClipPlane;
		float farClipPlane;
	};

	struct ViewportData
	{
		std::size_t m_width;
		std::size_t m_height;
	};

	struct RenderContext
	{
		CameraData m_cameraData;
		ViewportData m_viewportData;
		std::shared_ptr<IFramebuffer> m_framebuffer;
	};
} // namespace engine