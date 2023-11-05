#pragma once

#include <string>
#include <engine/debug/logging/debug_logger.h>

class GLFWwindow;

namespace engine
{
	namespace render
	{
		class Window final
		{
		private:
			using Logger = engine::debug::Logger;

		public:
			explicit Window(size_t width, size_t height, std::string label = "", bool isResizable = true, GLFWwindow* sharedContext = nullptr);

			size_t width() const;
			size_t height() const;
			bool isResizable() const;

		private:
			size_t m_width;
			size_t m_height;
			bool m_isResizable;

			GLFWwindow* m_window;
		};
	} // namespace render
} // namespace engine