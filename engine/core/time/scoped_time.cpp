#include "scoped_time.h"

#include <engine/core/time/time_manager.h>

namespace engine
{
	ScopedTime::ScopedTime(TimeManager* manager)
		: m_manager(manager)
	{
		m_now = std::chrono::high_resolution_clock::now();
	}

	ScopedTime::~ScopedTime()
	{
		std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
		m_manager->setDeltaTime(end - m_now);
	}
} // namespace engine