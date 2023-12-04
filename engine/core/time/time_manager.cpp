#include "time_manager.h"

namespace engine
{
    class TimeManager::Internal final
    {
    private:
        friend class TimeManager;

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

    TimeManager::TimeManager()
    {
        m_internal = std::make_unique<Internal>();
    }

    TimeManager::~TimeManager()
    {}

    void TimeManager::setDeltaTime(std::chrono::steady_clock::duration time)
    {
        m_internal->setDeltaTime(time);
    }

    double TimeManager::deltaTime() const
    {
        return m_internal->deltaTime();
    }

    double TimeManager::timeSinceLaunch() const
    {
        return m_internal->timeSinceLaunch();
    }
} // namespace engine