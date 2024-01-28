#include "editor_viewport_window.h"

#include <format>

#include <engine/core/input/input_system.h>
#include <engine/core/math/vector3.h>

#include <engine/editor/viewport/editor_framebuffer.h>

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
	constexpr std::uint32_t DEFAULT_VIEWPORT_WIDTH = 512;
	constexpr std::uint32_t DEFAULT_VIEWPORT_HEIGHT = 512;

	EditorViewportWindow::EditorViewportWindow(std::shared_ptr<IFramebuffer> framebuffer,
											   EditorViewportsManager* viewportsManager,
											   std::size_t viewportIndex)
		: m_isHovered(false)
		, m_isFocused(false)
		, m_framebuffer(framebuffer)
		, m_viewportsManager(viewportsManager)
		, m_viewportIndex(viewportIndex)
		, m_viewport(EditorViewport(DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT))
	{}

	EditorViewportWindow::EditorViewportWindow()
		: m_isHovered(false)
		, m_isFocused(false)
		, m_framebuffer(nullptr)
		, m_viewportsManager(nullptr)
		, m_viewportIndex(static_cast<std::size_t>(-1))
		, m_viewport(EditorViewport())
	{}

	bool EditorViewportWindow::isHovered() const
	{
		return m_isHovered;
	}

	bool EditorViewportWindow::isFocused() const
	{
		return m_isFocused;
	}

	void EditorViewportWindow::focus()
	{
		m_wantFocus = true;
	}

	EditorViewport& EditorViewportWindow::viewport()
	{
		return m_viewport;
	}

	void EditorViewportWindow::draw()
	{
		std::string viewportName = std::format("Viewport {}", m_viewportIndex);

		if (ImGui::Begin(viewportName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse))
		{
			if (m_wantFocus)
			{
				ImGui::SetWindowFocus();
			}

			EditorFramebuffer* framebuffer = static_cast<EditorFramebuffer*>(m_framebuffer.get());
			ImVec2 size = ImVec2(framebuffer->width(), framebuffer->height());
			ImGui::Image(framebuffer->textureID(), size);

			m_isHovered = ImGui::IsWindowHovered();
			m_isFocused = ImGui::IsWindowFocused();

			ImVec2 currentSize = ImGui::GetWindowSize();
			if (framebuffer->setSize(currentSize.x, currentSize.y))
			{
				m_viewport.setAspects(currentSize.x, currentSize.y);
			}
		}
		ImGui::End();
	}

	void EditorViewportWindow::handleCameraInput(Vector2 mouseMotion, Vector3 movementInput,
												 float movementSpeed, Vector2 sensitivity)
	{
		float yaw = -mouseMotion.x * sensitivity.x;
		float pitch = -mouseMotion.y * sensitivity.y;
		
		m_viewport.rotateCameraBy(pitch, yaw);

		Vector3 cameraForward = m_viewport.cameraForward();
		Vector3 cameraRight = glm::normalize(glm::cross(cameraForward, Vector3::down()));
		Vector3 cameraUp = glm::normalize(glm::cross(cameraForward, cameraRight));
		
		Vector3 cameraPosition = m_viewport.cameraPosition();
		cameraPosition += movementInput.x * movementSpeed * cameraRight;
		cameraPosition += movementInput.y * movementSpeed * cameraUp;
		cameraPosition += movementInput.z * movementSpeed * cameraForward;

		m_viewport.setCameraPosition(cameraPosition);
	}

	EditorViewportWindow& EditorViewportsManager::createViewport(std::shared_ptr<IFramebuffer> framebuffer,
																 std::shared_ptr<InputSystem> inputSystem)
	{
		std::size_t viewportIndex = m_viewports.getNextEmptyIndex() + 1;
		EditorViewportWindow viewport(std::move(framebuffer), this, viewportIndex);

		DEBUG_LOG("An editor viewport has been created.");
		return m_viewports.push(std::move(viewport))->get();
	}

	const EditorViewportsManager::Viewports& EditorViewportsManager::viewports()
	{
		return m_viewports;
	}
} // namespace engine
