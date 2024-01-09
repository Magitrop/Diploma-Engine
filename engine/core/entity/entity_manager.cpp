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

	ComponentID EntityManager::attachComponentInternal(EntityID entity, std::size_t uniqueComponentID)
	{
		return m_internal->attachComponent(entity, uniqueComponentID);
	}

	void EntityManager::detachComponentInternal(EntityID entity, std::size_t uniqueComponentID)
	{
		m_internal->detachComponent(entity, uniqueComponentID);
	}

	ComponentID EntityManager::getComponentInternal(EntityID entity, std::size_t uniqueComponentID)
	{
		return m_internal->getComponent(entity, uniqueComponentID);
	}

	std::shared_ptr<ComponentManager> EntityManager::getComponentManagerInternal(std::size_t uniqueComponentID)
	{
		return m_internal->getComponentManager(uniqueComponentID);
	}

	bool EntityManager::hasComponentInternal(EntityID entity, std::size_t uniqueComponentID)
	{
		return m_internal->hasComponent(entity, uniqueComponentID);
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