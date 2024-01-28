#include "editor_viewport.h"

#include <algorithm>

#include <engine/dependencies/glm/glm/ext/matrix_clip_space.hpp>
#include <engine/dependencies/glm/glm/ext/matrix_transform.hpp>
#include <engine/dependencies/glm/glm/geometric.hpp>
#include <engine/dependencies/glm/glm/trigonometric.hpp>

namespace engine
{
	constexpr Vector3 DEFAULT_CAMERA_POSITION = Vector3::zero();
	constexpr Vector3 DEFAULT_CAMERA_ANGLES = Vector3::zero();
	constexpr Vector3 DEFAULT_CAMERA_FORWARD = Vector3::forward();
	constexpr float DEFAULT_FOV = 60.0f;
	constexpr float DEFAULT_NEAR_CLIP_PLANE = 0.1f;
	constexpr float DEFAULT_FAR_CLIP_PLANE = 100.0f;

	EditorViewport::EditorViewport(std::uint32_t viewportWidth, std::uint32_t viewportHeight)
		: m_cameraPosition(DEFAULT_CAMERA_POSITION)
		, m_cameraAngles(DEFAULT_CAMERA_ANGLES)
		, m_cameraForward(DEFAULT_CAMERA_FORWARD)
		, m_fov(DEFAULT_FOV)
		, m_nearClipPlane(DEFAULT_NEAR_CLIP_PLANE)
		, m_farClipPlane(DEFAULT_FAR_CLIP_PLANE)
		, m_viewportWidth(viewportWidth)
		, m_viewportHeight(viewportHeight)
	{
		updateProjectionMatrix();
		updateViewMatrix();
	}

	EditorViewport::EditorViewport()
		: m_cameraPosition(DEFAULT_CAMERA_POSITION)
		, m_cameraAngles(DEFAULT_CAMERA_ANGLES)
		, m_cameraForward(DEFAULT_CAMERA_FORWARD)
		, m_fov(DEFAULT_FOV)
		, m_nearClipPlane(DEFAULT_NEAR_CLIP_PLANE)
		, m_farClipPlane(DEFAULT_FAR_CLIP_PLANE)
		, m_viewportWidth(0)
		, m_viewportHeight(0)
	{}

	void EditorViewport::rotateCameraBy(float pitch, float yaw, float roll /* = 0.0f */)
	{
		m_cameraAngles.x += yaw;
		m_cameraAngles.y += pitch;
		m_cameraAngles.z += roll;

		m_cameraAngles.y = std::clamp(m_cameraAngles.y, -89.0f, 89.0f);
		updateViewMatrix();
	}

	void EditorViewport::setCameraPosition(Vector3 position)
	{
		m_cameraPosition = position;
		updateViewMatrix();
	}

	void EditorViewport::setCameraAngles(Vector3 angles)
	{
		m_cameraAngles = angles;
		updateViewMatrix();
	}

	void EditorViewport::setFov(float fov)
	{
		m_fov = fov;
		updateProjectionMatrix();
	}

	void EditorViewport::setFarClipPlane(float plane)
	{
		m_farClipPlane = plane;
		updateProjectionMatrix();
	}

	void EditorViewport::setNearClipPlane(float plane)
	{
		m_nearClipPlane = plane;
		updateProjectionMatrix();
	}

	void EditorViewport::setAspects(std::uint32_t width, std::uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
		updateProjectionMatrix();
	}

	void EditorViewport::updateProjectionMatrix()
	{
		float aspectRatio = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
		m_projectionMatrix = glm::perspective(
			glm::radians(m_fov),
			aspectRatio,
			m_nearClipPlane,
			m_farClipPlane
		);
	}

	void EditorViewport::updateViewMatrix()
	{
		float pitch = glm::radians(m_cameraAngles.x);
		float yaw = glm::radians(m_cameraAngles.y);
		m_cameraForward.x = cos(pitch) * cos(yaw);
		m_cameraForward.y = sin(yaw);
		m_cameraForward.z = sin(pitch) * cos(yaw);
		m_cameraForward = glm::normalize(m_cameraForward);

		m_viewMatrix = glm::lookAt(
			m_cameraPosition,
			m_cameraPosition + m_cameraForward,
			Vector3::down()
		);
	}
} // namespace engine