#include "component_internal_base.h"

namespace engine
{
    void ComponentInternal::setComponentName(std::string name)
    {
        m_componentName = name;
    }

    void ComponentInternal::setUniqueComponentID(std::size_t id)
    {
        m_uniqueComponentID = id;
    }

    std::string ComponentInternal::getComponentName() const
    {
        return m_componentName;
    }

    std::size_t ComponentInternal::getUniqueComponentID() const
    {
        return m_uniqueComponentID;
    }
} // namespace engine