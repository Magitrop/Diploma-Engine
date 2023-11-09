#pragma once

#include <engine/internal/helpers/helpers/construction_helper.h>
#include <engine/render/window/window.h>

#include <string>
#include <vector>

class GLFWwindow;

namespace engine
{
	namespace executable
	{
		class Runtime;
	} // namespace executable

	namespace render
	{
		class Window;
	} // namespace render

	namespace internal
	{
		namespace render
		{
			class WindowManager final
			{
				friend Constructible<WindowManager>;

			private:
				using Window = engine::render::Window;
				using Runtime = engine::executable::Runtime;

			private:
				WindowManager() = default;

			public:
				using Constructor = Constructible<WindowManager>::ConstructibleBy<Runtime>;

			private:
				const Window& createWindow(std::size_t width,
										   std::size_t height,
										   std::string label = "",
										   bool isResizable = true,
										   GLFWwindow* sharedContext = nullptr);
				const Window& getWindowByID(std::size_t id) const;

				void setWindowAsCurrentContext(const Window& window);

				std::vector<Window> m_createdWindows;
			};
		} // namespace render
	} // namespace internal
} // namespace engine