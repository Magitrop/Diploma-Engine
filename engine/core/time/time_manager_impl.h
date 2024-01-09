#pragma once

#include <engine/core/time/time_manager.h>

namespace engine
{
    class TimeManager::Internal final
    {
    public:
        Internal() = default;
        ~Internal() = default;

        void setDeltaTime(std::chrono::steady_clock::duration time);
        double deltaTime() const;
        double timeSinceLaunch() const;

    private:
        std::chrono::steady_clock::duration m_deltaTime;
        std::chrono::steady_clock::duration m_timeSinceLaunch;
    };
} // namespace engine