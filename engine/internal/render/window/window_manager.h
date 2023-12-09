#pragma once

#include <memory>
#include <vector>

#include <engine/render/window/window.h>

namespace engine
{
	// A factory and container for Window objects.
	class WindowManager final
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		explicit WindowManager();

	public:
		~WindowManager();

		const Window* createWindow(std::size_t width,
								   std::size_t height,
								   std::string label = "",
								   bool isResizable = true,
								   const Window* sharedContext = nullptr);
		const Window* getWindowByID(std::size_t id) const;

		void setWindowAsCurrentContext(const Window* window);
		void setWindowTitle(const Window* window, const char* title);
		void swapBuffers(const Window* window);

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine