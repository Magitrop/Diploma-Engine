#include "window.h"

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	WindowID::WindowID(std::size_t index)
		: m_id(index)
	{}

	WindowID::WindowID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	std::size_t WindowID::id() const
	{
		return m_id;
	}

	WindowID::operator std::size_t() const
	{
		return m_id;
	}

	bool WindowID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}
} // namespace engine