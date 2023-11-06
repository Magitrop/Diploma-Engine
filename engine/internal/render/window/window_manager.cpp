#include "window_manager.h"

#include <engine/render/window/window.h>

using WindowManager = engine::internal::render::WindowManager;

std::shared_ptr<const WindowManager::Window> WindowManager::createWindow(std::size_t width,
																		 std::size_t height,
																		 std::string label /* = "" */,
																		 bool isResizable /* = true */,
																		 GLFWwindow* sharedContext /* = nullptr */)
{
    return m_createdWindows.emplace_back(Window::constructShared(width, height, label, isResizable, sharedContext));
}

std::shared_ptr<const WindowManager::Window> WindowManager::getWindowByID(std::size_t id) const
{
    if (id < m_createdWindows.size())
        return m_createdWindows.at(id);
    return nullptr;
}
