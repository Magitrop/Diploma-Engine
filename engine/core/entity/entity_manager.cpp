#include "entity_manager.h"

#include <bitset>
#include <unordered_map>

#include <engine/core/components/component_manager.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/entity/entity_manager_impl.h>
#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	EntityManager::EntityManager(std::shared_ptr<ComponentRegistrar> registrar)
	{
		m_internal = std::make_unique<Internal>(std::move(registrar));
	}

	EntityManager::~EntityManager()
	{}

	ComponentID EntityManager::attachComponentInternal(EntityID entity, std::string componentName)
	{
		std::size_t uniqueID = m_internal->registrar()->getComponentIDByName(componentName);
		return m_internal->attachComponent(entity, uniqueID);
	}

	void EntityManager::detachComponentInternal(EntityID entity, std::string componentName)
	{
		std::size_t uniqueID = m_internal->registrar()->getComponentIDByName(componentName);
		m_internal->detachComponent(entity, uniqueID);
	}

	ComponentID EntityManager::getComponentInternal(EntityID entity, std::string componentName)
	{
		std::size_t uniqueID = m_internal->registrar()->getComponentIDByName(componentName);
		return m_internal->getComponent(entity, uniqueID);
	}

	std::shared_ptr<ComponentManager> EntityManager::getComponentManagerInternal(std::string componentName)
	{
		std::size_t uniqueID = m_internal->registrar()->getComponentIDByName(componentName);
		return m_internal->getComponentManager(uniqueID);
	}

	bool EntityManager::hasComponentInternal(EntityID entity, std::string componentName)
	{
		std::size_t uniqueID = m_internal->registrar()->getComponentIDByName(componentName);
		return m_internal->hasComponent(entity, uniqueID);
	}

	EntityID EntityManager::createEntity()
	{
		return m_internal->createEntity();
	}

	void EntityManager::destroyEntity(EntityID entity)
	{
		m_internal->destroyEntity(entity);
	}
} // namespace engine