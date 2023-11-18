#include "glad_graphic_api.h"

#include <engine/dependencies/gl/glad/include/glad/glad.h>
#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	bool GladGraphicAPI::initialize()
	{
		// TODO: log errors
		if (!gladLoadGL())
		{
			//std::cout << "gladLoadGL failed" << std::endl;
			return false;
		}
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}
		//std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		//std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		return true;
	}
} // namespace engine
