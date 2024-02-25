#include "component_manager.h"

#include <engine/core/components/component_manager_impl.h>
#include <engine/core/components/transform_component_impl.h>

namespace engine
{
	ComponentManager::ComponentManager()
	{
		m_internal = std::make_unique<Internal>();
	}

	ComponentManager::~ComponentManager() {}

	std::string ComponentManager::getComponentName() const
	{
		return m_internal ? m_internal->getComponentName() : constants::UNREGISTERED_COMPONENT_NAME;
	}

	std::size_t ComponentManager::getUniqueComponentID() const
	{
		return m_internal ? m_internal->getUniqueComponentID() : constants::UNREGISTERED_UNIQUE_COMPONENT_ID;
	}

	std::unique_ptr<ComponentManager::Internal> ComponentManager::m_internal;
} // namespace engine