#pragma once

#include <chrono>

namespace engine
{
	class TimeManager final
	{
		// friends
	private:
		friend class ScopedTime;

	public:
		explicit TimeManager();
		~TimeManager();

		double deltaTime() const;
		double timeSinceLaunch() const;

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine