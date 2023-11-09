#pragma once

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/helpers/construction_helper.h>

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
			// usings
		private:
			using WindowManager = engine::internal::render::WindowManager;

		public:
			using Constructor = Constructible<Window>::ConstructibleBy<WindowManager>;

			// friends
		private:
			friend Constructible<Window>;
			friend class WindowManager;

			// members
		private:
			explicit Window(std::size_t width,
							std::size_t height,
							std::string label = "", 
							bool isResizable = true, 
							GLFWwindow* sharedContext = nullptr);
			explicit Window();

		public:
			~Window();

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