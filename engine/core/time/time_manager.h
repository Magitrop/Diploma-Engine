#pragma once

#include <chrono>

namespace engine
{
	class TimeManager
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;
		friend class ScopedTime;

		// members
	private:
		explicit TimeManager();

		void setDeltaTime(std::chrono::steady_clock::duration time);

	public:
		~TimeManager();

		double deltaTime() const;
		double timeSinceLaunch() const;

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine