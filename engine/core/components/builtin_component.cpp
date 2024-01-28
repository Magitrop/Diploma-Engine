#include "builtin_component.h"

#include <engine/core/components/component_internal_base.h>

namespace engine
{
    std::string BuiltinComponent::getComponentName() const
    {
        return m_internal->getComponentName();
    }

    std::size_t BuiltinComponent::getUniqueComponentID() const
    {
        return m_internal->getUniqueComponentID();
    }
} // namespace engine