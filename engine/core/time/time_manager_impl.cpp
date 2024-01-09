#include "time_manager_impl.h"

namespace engine
{
    void TimeManager::Internal::setDeltaTime(std::chrono::steady_clock::duration time)
    {
        m_deltaTime = time;
        m_timeSinceLaunch += time;
    }

    double TimeManager::Internal::deltaTime() const
    {
        return std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_deltaTime)).count();
    }

    double TimeManager::Internal::timeSinceLaunch() const
    {
        return std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_timeSinceLaunch)).count();
    }
} // namespace engine