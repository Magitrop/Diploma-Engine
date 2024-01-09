#include "component_manager_impl.h"

namespace engine
{
    std::string engine::ComponentManager::Internal::getComponentName()
    {
        return m_componentName;
    }

    void engine::ComponentManager::Internal::setComponentName(std::string name)
    {
        m_componentName = name;
    }

    std::size_t ComponentManager::Internal::getUniqueComponentID()
    {
        return m_uniqueComponentID;
    }

    void ComponentManager::Internal::setUniqueComponentID(std::size_t id)
    {
        m_uniqueComponentID = id;
    }
} // namespace engine