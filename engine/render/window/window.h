#pragma once

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/render/window/window_manager.h>

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
		class Window final
		{
		private:
			friend class engine::internal::render::WindowManager;

			explicit Window(
				std::size_t width,
				std::size_t height,
				std::string label = "", 
				bool isResizable = true, 
				GLFWwindow* sharedContext = nullptr);
			~Window();

		public:
			void setAsCurrentContext() const;

			std::size_t width() const;
			std::size_t height() const;
			bool isResizable() const;

		private:
			std::size_t m_width;
			std::size_t m_height;
			bool m_isResizable;

			GLFWwindow* m_window;
		};
	} // namespace render
} // namespace engine