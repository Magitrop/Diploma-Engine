#pragma once

#include <string>

#include <engine/debug/logging/debug_logger.h>

class GLFWwindow;

namespace engine
{
	// A lightweight wrapper for an actual window in the Window Manager.
	class WindowID final
	{
	public:
		explicit WindowID(std::size_t index);

		// Creates an invalid WindowID.
		explicit WindowID();

		std::size_t id() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};
} // namespace engine