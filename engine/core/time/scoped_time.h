#pragma once

#include <chrono>

namespace engine
{
	class TimeManager;
	class ScopedTime
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		ScopedTime(TimeManager* manager);

	public:
		~ScopedTime();

	private:
		std::chrono::steady_clock::time_point m_now;
		TimeManager* m_manager;
	};
} // namespace engine