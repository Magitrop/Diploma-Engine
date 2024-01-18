#include "editor_viewport_window.h"

#include <engine/core/input/input_system.h>
#include <engine/editor/viewport/editor_framebuffer.h>

#include <engine/dependencies/glm/glm/geometric.hpp>
#include <engine/dependencies/glm/glm/trigonometric.hpp>

namespace engine
{
	EditorViewportWindow::EditorViewportWindow(std::shared_ptr<InputSystem> inputSystem,
											   EditorCamera camera,
											   std::size_t index)
		: m_inputSystem(inputSystem)
		, m_editorCamera(camera)
		, m_viewportIndex(index)
	{}

	void EditorViewportWindow::draw() const
	{
		std::string viewportName = std::format("Viewport {}", m_viewportIndex);

		if (ImGui::Begin(viewportName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse))
		{
			EditorFramebuffer* framebuffer = static_cast<EditorFramebuffer*>(m_editorCamera.framebuffer.get());
			ImVec2 size = ImVec2(m_editorCamera.framebuffer->width(), m_editorCamera.framebuffer->height());
			ImGui::Image(framebuffer->textureID(), size);

			if (m_inputSystem->onMouseButtonDown(MouseButton::Right))
			{
				if (ImGui::IsWindowHovered())
				{
					ImGui::SetWindowFocus();
				}
			}

			if (m_inputSystem->isMouseButtonPressed(MouseButton::Right))
			{
				if (ImGui::IsWindowFocused())
				{
					Vector2 mouse = m_inputSystem->mouseMotion();

					float yaw = mouse.x * EditorCamera::CAMERA_SENSITIVITY.x;
					float pitch = mouse.y * EditorCamera::CAMERA_SENSITIVITY.y;

					m_editorCamera.cameraAngles.x -= yaw;
					m_editorCamera.cameraAngles.y -= pitch;
					m_editorCamera.cameraAngles.y = std::clamp(m_editorCamera.cameraAngles.y, -89.0f, 89.0f);

					Vector3 cameraForward;
					cameraForward.x = cos(glm::radians(m_editorCamera.cameraAngles.x)) * cos(glm::radians(m_editorCamera.cameraAngles.y));
					cameraForward.y = sin(glm::radians(m_editorCamera.cameraAngles.y));
					cameraForward.z = sin(glm::radians(m_editorCamera.cameraAngles.x)) * cos(glm::radians(m_editorCamera.cameraAngles.y));
					cameraForward = glm::normalize(cameraForward);

					if (m_inputSystem->isKeyPressed(KeyCode::W))
					{
						m_editorCamera.cameraPosition += cameraForward * m_editorCamera.CAMERA_SPEED;
					}
					else if (m_inputSystem->isKeyPressed(KeyCode::S))
					{
						m_editorCamera.cameraPosition -= cameraForward * m_editorCamera.CAMERA_SPEED;
					}

					if (m_inputSystem->isKeyPressed(KeyCode::D))
					{
						Vector3 cameraLeft = glm::normalize(glm::cross(cameraForward, Vector3::up()));
						m_editorCamera.cameraPosition -= cameraLeft * m_editorCamera.CAMERA_SPEED;
					}
					else if (m_inputSystem->isKeyPressed(KeyCode::A))
					{
						Vector3 cameraLeft = glm::normalize(glm::cross(cameraForward, Vector3::up()));
						m_editorCamera.cameraPosition += cameraLeft * m_editorCamera.CAMERA_SPEED;
					}
				}
			}

			ImVec2 currentSize = ImGui::GetWindowSize();
			framebuffer->setSize(currentSize.x, currentSize.y);
		}
		ImGui::End();
	}

	constexpr Vector3 DEFAULT_EYE_POSITION = Vector3::zero();
	constexpr Vector3 DEFAULT_EYE_ANGLES = Vector3::zero();
	constexpr float DEFAULT_FOV = 60.0f;
	constexpr float DEFAULT_NEAR_CLIP_PLANE = 0.1f;
	constexpr float DEFAULT_FAR_CLIP_PLANE = 100.0f;

	EditorViewportWindow EditorViewports::createViewport(std::shared_ptr<IFramebuffer> framebuffer,
														 std::shared_ptr<InputSystem> inputSystem)
	{
		EditorCamera camera
		{
			m_eyePosition.push(DEFAULT_EYE_POSITION)->get(),
			m_eyeAngles.push(DEFAULT_EYE_ANGLES)->get(),
			m_fov.push(DEFAULT_FOV)->get(),
			m_nearClipPlane.push(DEFAULT_NEAR_CLIP_PLANE)->get(),
			m_farClipPlane.push(DEFAULT_FAR_CLIP_PLANE)->get(),
			m_framebuffer.push(std::move(framebuffer))->get()
		};

		std::shared_ptr<EditorViewportWindow> window(new EditorViewportWindow(std::move(inputSystem), camera, m_framebuffer.size()));
		DEBUG_LOG("An Editor Viewport has been created.");
		return *m_viewports.push(window)->get();
	}

	void EditorViewports::draw() const
	{
		for (const auto& viewport : m_viewports)
		{
			viewport.get()->draw();
		}
	}

	const EditorCameraIterator EditorViewports::begin() const
	{
		return
		{
			m_eyePosition.begin(),
			m_eyeAngles.begin(),
			m_fov.begin(),
			m_nearClipPlane.begin(),
			m_farClipPlane.begin(),
			m_framebuffer.begin()
		};
	}

	const EditorCameraIterator EditorViewports::end() const
	{
		return
		{
			m_eyePosition.end(),
			m_eyeAngles.end(),
			m_fov.end(),
			m_nearClipPlane.end(),
			m_farClipPlane.end(),
			m_framebuffer.end()
		};
	}

	EditorCameraIterator& EditorCameraIterator::operator ++ ()
	{
		++cameraPositionIt;
		++cameraAnglesIt;
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
		result &= cameraAnglesIt	!= other.cameraAnglesIt;
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
			cameraAnglesIt->get(),
			fovIt->get(),
			nearClipPlaneIt->get(),
			farClipPlaneIt->get(),
			framebufferIt->get(),
		};
	}
} // namespace engine
