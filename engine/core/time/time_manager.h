#pragma once

#include <chrono>

namespace engine
{
	class TimeManager
	{
		// friends
	private:
		friend class ScopedTime;

		// members
	private:
		void setDeltaTime(std::chrono::steady_clock::duration time);

	public:
		double deltaTime() const;
		double timeSinceLaunch() const;

	private:
		std::chrono::steady_clock::duration m_deltaTime;
		std::chrono::steady_clock::duration m_timeSinceLaunch;
	};
} // namespace engine