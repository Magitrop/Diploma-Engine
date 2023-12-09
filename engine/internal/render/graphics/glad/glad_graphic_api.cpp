#include "glad_graphic_api.h"

#include <sstream>

#include <engine/debug/logging/debug_logger.h>

#include <engine/dependencies/gl/glad/include/glad/glad.h>
#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	class GladGraphicAPI::Internal final
	{
	public:
		Internal() = default;
		~Internal() = default;

		bool initialize();
		void finalize();

	private:

	};

	bool GladGraphicAPI::Internal::initialize()
	{
		if (!gladLoadGL())
		{
			FATAL_LOG("Glad initialization failed. Further initialization is ceased.");
			return false;
		}
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			FATAL_LOG("Glad initialization failed. Further initialization is ceased.");
			return false;
		}

		{
			std::stringstream s;
			s << glGetString(GL_RENDERER);
			INFO_LOG("Renderer: {}", s.str());
		}
		{
			std::stringstream s;
			s << glGetString(GL_VERSION);
			INFO_LOG("OpenGL Version: {}", s.str());
		}

		return true;
	}

	GladGraphicAPI::GladGraphicAPI()
	{
		m_internal = std::make_unique<Internal>();
	}

	GladGraphicAPI::~GladGraphicAPI()
	{}

	void GladGraphicAPI::Internal::finalize() {}

	void GladGraphicAPI::finalize()
	{
		m_internal->finalize();
	}

	bool GladGraphicAPI::initialize()
	{
		return m_internal->initialize();
	}
} // namespace engine
