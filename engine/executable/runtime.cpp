#include "runtime.h"

#include <engine/editor/initialization/editor_initializer.h>
#include <engine/internal/render/gl/glad/include/glad/glad.h>
#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>
#include <engine/internal/render/window/window_manager.h>

using Runtime = engine::executable::Runtime;

void Runtime::initialize()
{
	initializeGL();
	initializeEditor();
	initializeWindowManager();
}

void Runtime::initializeGL()
{
	glfwInit();
	gladLoadGL();
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Runtime::initializeEditor()
{
	editorInitializer = EditorInitializer::constructShared();
}

void Runtime::initializeWindowManager()
{
	windowManager = WindowManager::constructShared();
}
