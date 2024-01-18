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
		Vector3& cameraAngles; // yaw, pitch, roll
		float& fov;
		float& nearClipPlane;
		float& farClipPlane;
		std::shared_ptr<IFramebuffer>& framebuffer;

		static constexpr Vector2 CAMERA_SENSITIVITY = Vector2(1.0f, 1.0f);
		static constexpr float CAMERA_SPEED = 0.05f;
	};
} // namespace engine
#endif // #if IS_EDITOR