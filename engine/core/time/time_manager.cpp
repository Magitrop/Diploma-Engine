#include "time_manager.h"

#include <engine/core/time/time_manager_impl.h>

namespace engine
{
    TimeManager::TimeManager()
    {
        m_internal = std::make_unique<Internal>();
    }

    TimeManager::~TimeManager()
    {}

    double TimeManager::deltaTime() const
    {
        return m_internal->deltaTime();
    }

    double TimeManager::timeSinceLaunch() const
    {
        return m_internal->timeSinceLaunch();
    }
} // namespace engine