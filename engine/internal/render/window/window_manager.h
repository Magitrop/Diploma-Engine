#pragma once

#include <engine/render/window/window.h>

#include <string>
#include <vector>

class GLFWwindow;

namespace engine
{
	namespace executable
	{
		class ProductionRuntimePipeline;
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
				// usings
			private:
				using Window = engine::render::Window;

				// friends
			private:
				friend engine::executable::ProductionRuntimePipeline;

				// members
			public:
				~WindowManager();

				const Window* createWindow(std::size_t width,
										   std::size_t height,
										   std::string label = "",
										   bool isResizable = true,
										   const Window* sharedContext = nullptr);
				const Window* getWindowByID(std::size_t id) const;

				void setWindowAsCurrentContext(const Window* window);

			private:
				std::list<Window> m_createdWindows;
			};
		} // namespace render
	} // namespace internal
} // namespace engine