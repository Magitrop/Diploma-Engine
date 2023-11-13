#pragma once

#include <engine/debug/logging/debug_logger.h>

#include <string>

class GLFWwindow;

namespace engine
{
	namespace internal
	{
		namespace render
		{
			class WindowManager;
		} // namespace render
	} // namespace internal

	namespace render
	{
		// A non-owning wrapper for the GLFW window.
		class Window final
		{
			// friends
		private:
			friend class engine::internal::render::WindowManager;

			// members
		private:
			explicit Window(std::size_t width,
							std::size_t height,
							std::string label = "", 
							bool isResizable = true,
							GLFWwindow* window = nullptr);
			explicit Window();

		public:
			std::size_t width() const;
			std::size_t height() const;
			bool isResizable() const;
			bool isValid() const;

		private:
			std::size_t m_width;
			std::size_t m_height;
			bool m_isResizable;
			bool m_isValid;

			GLFWwindow* m_window;
		};
	} // namespace render
} // namespace engine