#pragma once

#include <engine/internal/helpers/helpers/construction_helper.h>
#include <engine/render/window/window.h>

#include <vector>

namespace engine
{
	namespace executable
	{
		class Runtime;
	} // namespace executable

	namespace internal
	{
		namespace render
		{
			class WindowManager final : ConstructionHelper<WindowManager>
			{
				friend class ConstructionHelper;
				friend class engine::executable::Runtime;

				using Window = engine::render::Window;

			private:
				WindowManager() = default;

				std::shared_ptr<const Window> createWindow(
					std::size_t width,
					std::size_t height,
					std::string label = "",
					bool isResizable = true,
					GLFWwindow* sharedContext = nullptr);
				std::shared_ptr<const Window> getWindowByID(std::size_t id) const;

				std::vector<std::shared_ptr<const Window>> m_createdWindows;
			};
		} // namespace render
	} // namespace internal
} // namespace engine