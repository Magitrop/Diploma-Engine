#pragma once

#include <chrono>

namespace engine
{
	class TimeManager;
	class ScopedTime final
	{
	public:
		ScopedTime(TimeManager* manager);
		~ScopedTime();

	private:
		std::chrono::steady_clock::time_point m_now;
		TimeManager* m_manager;
	};
} // namespace engine