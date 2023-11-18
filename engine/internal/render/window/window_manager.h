#pragma once

#include <engine/render/window/window.h>
#include <engine/internal/helpers/persistent_vector.h>

#include <string>
#include <vector>

class GLFWwindow;

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

	private:
		PersistentVector<Window, 8> m_createdWindows; // it is rather unlikely to have more than 8 windows at once
	};
} // namespace engine