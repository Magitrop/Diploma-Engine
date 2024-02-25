#include "editor_viewport_window.h"

#include <format>

#include <engine/core/input/input_system.h>
#include <engine/core/math/vector3.h>

#include <engine/debug/memory/memory_guard.h>

#include <engine/editor/gui/imgui_scoped_guards.h>
#include <engine/editor/viewport/editor_framebuffer.h>

#include <engine/dependencies/glm/glm/ext/matrix_clip_space.hpp>
#include <engine/dependencies/glm/glm/ext/matrix_transform.hpp>
#include <engine/dependencies/glm/glm/geometric.hpp>
#include <engine/dependencies/glm/glm/trigonometric.hpp>
#include <engine/internal/render/glad/glad_framebuffer.h>

namespace engine
{
	constexpr std::uint32_t DEFAULT_VIEWPORT_WIDTH = 512;
	constexpr std::uint32_t DEFAULT_VIEWPORT_HEIGHT = 512;

	EditorViewportWindow::EditorViewportWindow(WindowFramebuffers framebuffers,
											   EditorViewportsManager* viewportsManager,
											   std::size_t viewportIndex)
		: m_isHovered(false)
		, m_isFocused(false)
		, m_windowFramebuffers(std::move(framebuffers))
		, m_viewportsManager(viewportsManager)
		, m_viewportIndex(viewportIndex)
		, m_viewport(EditorViewport(DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT))
	{}

	EditorViewportWindow::EditorViewportWindow()
		: m_isHovered(false)
		, m_isFocused(false)
		, m_windowFramebuffers({ nullptr, nullptr })
		, m_viewportsManager(nullptr)
		, m_viewportIndex(static_cast<std::size_t>(-1))
		, m_viewport(EditorViewport())
		, m_imguiWindowProperties(WindowProperties())
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

	Vector2 EditorViewportWindow::screenToContent(Vector2 screenCoord) const
	{
		return screenCoord - properties().position - properties().topLeft;
	}

	void EditorViewportWindow::draw()
	{
		std::string viewportName = std::format("Viewport {}", m_viewportIndex);

		ImGuiScopedStyleVar var(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin(viewportName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse))
		{
			if (m_wantFocus)
			{
				ImGui::SetWindowFocus();
				m_wantFocus = false;
			}

			EditorFramebuffer* viewportFramebuffer = static_cast<EditorFramebuffer*>(framebuffers().viewportFramebuffer.get());
			EditorFramebuffer* selectionFramebuffer = static_cast<EditorFramebuffer*>(framebuffers().selectionFramebuffer.get());
			ImVec2 size = ImVec2(viewportFramebuffer->width(), viewportFramebuffer->height());
			ImGui::Image(viewportFramebuffer->textureID(), size);

			if (m_isFocused = ImGui::IsWindowFocused())
				m_viewportsManager->m_focusedViewport = this;

			if (m_isHovered = ImGui::IsWindowHovered())
				m_viewportsManager->m_hoveredViewport = this;

			ImVec2 currentSize = ImGui::GetWindowSize();
			if (viewportFramebuffer->setSize(currentSize.x, currentSize.y) &&
				selectionFramebuffer->setSize(currentSize.x, currentSize.y))
			{
				m_viewport.setAspects(currentSize.x, currentSize.y);
			}

			ImVec2 pos = ImGui::GetWindowPos();
			ImVec2 topLeft = ImGui::GetWindowContentRegionMin();
			ImVec2 bottomRight = ImGui::GetWindowContentRegionMax();
			m_imguiWindowProperties =
			{
				Vector2(pos.x, pos.y),
				Vector2(topLeft.x, topLeft.y),
				Vector2(bottomRight.x, bottomRight.y)
			};
		}
		ImGui::End();
	}

	void EditorViewportWindow::handleCameraInput(Vector2 mouseMotion, Vector3 movementInput,
												 float movementSpeed, Vector2 sensitivity)
	{
		// TODO: forbid any actions when dragging gizmo
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

	void EditorViewportsManager::onBeforeDraw()
	{
		m_hoveredViewport = nullptr;
		m_focusedViewport = nullptr;
	}

	EditorViewportWindow& EditorViewportsManager::createViewport(EditorViewportWindow::WindowFramebuffers framebuffers,
																 std::shared_ptr<InputSystem> inputSystem)
	{
		MEMORY_GUARD;

		std::size_t viewportIndex = m_viewports.getNextEmptyIndex() + 1;
		EditorViewportWindow viewport(std::move(framebuffers), this, viewportIndex);

		DEBUG_LOG("An editor viewport has been created.");
		return m_viewports.push(std::move(viewport))->get();
	}

	const EditorViewportsManager::Viewports& EditorViewportsManager::viewports()
	{
		return m_viewports;
	}

	const EditorViewportWindow* EditorViewportsManager::focusedViewport() const
	{
		return m_focusedViewport;
	}

	const EditorViewportWindow* EditorViewportsManager::hoveredViewport() const
	{
		return m_hoveredViewport;
	}
} // namespace engine
