#include "time_manager.h"

namespace engine
{
    void TimeManager::setDeltaTime(std::chrono::steady_clock::duration time)
    {
        m_deltaTime = time;
        m_timeSinceLaunch += time;
    }

    double TimeManager::deltaTime() const
    {
        return std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_deltaTime)).count();
    }

    double TimeManager::timeSinceLaunch() const
    {
        return std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_timeSinceLaunch)).count();
    }
} // namespace engine