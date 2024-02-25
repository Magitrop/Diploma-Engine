#pragma once

#include <memory>

#include <engine/core/math/matrix4x4.h>
#include <engine/core/math/ray.h>
#include <engine/core/math/vector3.h>

namespace engine
{
	class EditorViewport final
	{
	private:
		friend class Editor;
		friend class EditorViewportsManager;

	public:
		explicit EditorViewport(std::uint32_t viewportWidth, std::uint32_t viewportHeight);
		// Creates an invalid viewport.
		explicit EditorViewport();

		Vector3 cameraPosition() const { return m_cameraPosition; }
		Vector3 cameraAngles() const { return m_cameraAngles; }
		Vector3 cameraForward() const { return m_cameraForward; }
		float fov() const { return m_fov; } // field of view
		float nearClipPlane() const { return m_nearClipPlane; }
		float farClipPlane() const { return m_farClipPlane; }

		Matrix4x4 projection() const { return m_projectionMatrix; }
		Matrix4x4 view() const { return m_viewMatrix; }

		void rotateCameraBy(float pitch, float yaw, float roll = 0.0f);

		void setCameraPosition(Vector3 position);
		void setCameraAngles(Vector3 angles);
		void setFov(float fov);
		void setFarClipPlane(float plane);
		void setNearClipPlane(float plane);

		std::uint32_t width() const { return m_viewportWidth; }
		void setWidth(std::uint32_t width);

		std::uint32_t height() const { return m_viewportHeight; }
		void setHeight(std::uint32_t height);

		float aspectRatio() const { return static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight); }
		void setAspects(std::uint32_t width, std::uint32_t height);

		// XY coordinates represent the position on the screen.
		// Z coordinate represents the hyperbolical distance between the camera and the point.
		Vector3 worldPointToScreen(Vector3 world) const;

		// XY coordinates represent the position on the screen.
		// Z coordinate represents the distance from the camera plane.
		Vector3 screenPointToWorld(Vector3 screen) const;
			
		// Converts a screen point to a world relative ray which starts in the near camera plane and goes to the far one.
		Ray screenPointToRay(Vector2 screen) const;

	private:
		void updateProjectionMatrix();
		void updateViewMatrix();

		Vector3 m_cameraPosition;
		Vector3 m_cameraAngles; // yaw, pitch, roll
		Vector3 m_cameraForward;
		float m_fov;
		float m_nearClipPlane;
		float m_farClipPlane;
		std::uint32_t m_viewportWidth;
		std::uint32_t m_viewportHeight;

		Matrix4x4 m_projectionMatrix{};
		Matrix4x4 m_viewMatrix{};
	};
} // namespace engine