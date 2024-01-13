#include "imgui_scoped_frame.h"

#include <engine/dependencies/glad/include/glad/glad.h>
#include <engine/dependencies/imgui/backends/imgui_impl_glfw.h>
#include <engine/dependencies/imgui/backends/imgui_impl_opengl3.h>
#include <engine/dependencies/imgui/imgui.h>

namespace engine
{
	ImGuiScopedFrame::ImGuiScopedFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	ImGuiScopedFrame::~ImGuiScopedFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
} // namespace engine