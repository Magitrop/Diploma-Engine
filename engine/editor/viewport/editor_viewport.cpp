#include "editor_viewport.h"

#include <algorithm>

#include <engine/core/math/vector4.h>

#include <engine/dependencies/glm/glm/ext/matrix_clip_space.hpp>
#include <engine/dependencies/glm/glm/ext/matrix_transform.hpp>
#include <engine/dependencies/glm/glm/geometric.hpp>
#include <engine/dependencies/glm/glm/trigonometric.hpp>

namespace engine
{
	constexpr Vector3 DEFAULT_CAMERA_POSITION = Vector3(-3, 0, 0); // TODO: reconsider default camera transform, make it dependent on last viewport camera
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

	void EditorViewport::setWidth(std::uint32_t width)
	{
		m_viewportWidth = width;
		updateProjectionMatrix();
	}

	void EditorViewport::setHeight(std::uint32_t height)
	{
		m_viewportHeight = height;
		updateProjectionMatrix();
	}

	void EditorViewport::setAspects(std::uint32_t width, std::uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
		updateProjectionMatrix();
	}

	Vector3 EditorViewport::worldPointToScreen(Vector3 world) const
	{
		Matrix4x4 projView = projection() * view();
		Vector4 pos = Vector4(world, 1.0f);

		pos = projView * pos;
		pos /= pos.w;
		pos.x = (pos.x + 1.0f) / 2.0f * width();
		pos.y = (pos.y + 1.0f) / 2.0f * height();
		pos.z = (pos.z + 1.0f) / 2.0f * (farClipPlane() - nearClipPlane());

		return pos;
	}

	Vector3 EditorViewport::screenPointToWorld(Vector3 screen) const
	{
		Matrix4x4 invProjView = glm::inverse(projection() * view());

		float x = 2 * (screen.x / width()) - 1;
		float y = 2 * (screen.y / height()) - 1;
		float z = 2 * (screen.z / (farClipPlane() - nearClipPlane())) - 1;

		Vector4 nearCoord = invProjView * Vector4(x, y, z, 1.0f);
		if (std::fabs(nearCoord.w) < 1e-5)
			nearCoord.w = 1e-5;
		nearCoord /= nearCoord.w;

		return nearCoord;
	}

	Ray EditorViewport::screenPointToRay(Vector2 screen) const
	{
		Vector3 start = screenPointToWorld(Vector3(screen, nearClipPlane()));
		Vector3 end = screenPointToWorld(Vector3(screen, farClipPlane()));
		return Ray(start, Vector3(end - start).normalized());
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