#include "entity_manager_impl.h"

#include <engine/core/components/builtin_component.h>
#include <engine/core/components/component_internal_base.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/components/transform_component.h>
#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>

namespace engine
{
	EntityManager::Internal::Internal(std::shared_ptr<ComponentRegistrar> registrar)
		: m_componentRegistrar(registrar)
	{}

	ComponentID EntityManager::Internal::attachComponent(EntityID entity, std::size_t uniqueComponentID)
	{
		MEMORY_GUARD;

		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot attach component. The component was not properly registered.");
			return ComponentID(static_cast<std::size_t>(-1));
		}

		auto& entityComponents = m_entities.at(entity)->get().components;
		auto& componentOwners = m_componentOwners[uniqueComponentID];

		// already attached
		if (entityComponents[uniqueComponentID])
		{
			WARNING_LOG("Cannot attach component. This entity already have one. To check if an entity possesses some component use hasComponent function.");
			return ComponentID(static_cast<std::size_t>(-1));
		}

		ComponentID componentID = ComponentID(componentOwners.push(entity).getIndex());
		entityComponents[uniqueComponentID] = 1;

		std::shared_ptr<ComponentManager> manager = getComponentManager(uniqueComponentID);
		if (auto builtin = std::dynamic_pointer_cast<BuiltinComponent>(manager))
		{
			builtin->m_internal->attachComponent(componentID);
		}
		manager->onComponentAttached(entity, componentID);

		return componentID;
	}

	void EntityManager::Internal::detachComponent(EntityID entity, std::size_t uniqueComponentID)
	{
		MEMORY_GUARD;

		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot detach component. The component was not properly registered.");
			return;
		}

		auto& entityComponents = m_entities.at(entity)->get().components;
		auto& componentOwners = m_componentOwners[uniqueComponentID];

		// already detached
		if (!entityComponents[uniqueComponentID])
		{
			WARNING_LOG("Cannot detach component. This entity does not have one. To check if an entity possesses some component use hasComponent function.");
			return;
		}

		auto owner = componentOwners.find(entity);

		ComponentID componentID = ComponentID(owner.getIndex());
		getComponentManager(uniqueComponentID)->onComponentDetached(entity, componentID);
		if (auto manager = std::dynamic_pointer_cast<BuiltinComponent>(getComponentManager(uniqueComponentID)))
		{
			manager->m_internal->detachComponent(componentID);
		}

		componentOwners.remove(owner);
		entityComponents[uniqueComponentID] = 0;
	}

	ComponentID EntityManager::Internal::getComponent(EntityID entity, std::size_t uniqueComponentID)
	{
		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot obtain component. The component was not properly registered.");
			return ComponentID(static_cast<std::size_t>(-1));
		}

		auto& componentOwners = m_componentOwners[uniqueComponentID];

		if (componentOwners.size() != 0)
		{
			std::size_t index = 0;
			for (const auto& owner : componentOwners)
			{
				if (owner.get() == entity)
					return ComponentID(index);
				++index;
			}
		}

		WARNING_LOG("Cannot get component. This entity does not have one. To check if an entity possesses some component use hasComponent function.");
		return ComponentID(static_cast<std::size_t>(-1));
	}

	bool EntityManager::Internal::hasComponent(EntityID entity, std::size_t uniqueComponentID)
	{
		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot check component. The component was not properly registered.");
			return false;
		}

		return m_entities.at(entity)->get().components[uniqueComponentID];
	}

	std::shared_ptr<ComponentManager> EntityManager::Internal::getComponentManager(std::size_t uniqueComponentID)
	{
		return m_componentRegistrar->getComponentManager(uniqueComponentID);
	}

	EntityID EntityManager::Internal::createEntity()
	{
		MEMORY_GUARD;

		EntityID entity = EntityID(m_entities.push(EntityWrapper()).getIndex());
		attachComponent(entity, registrar()->getComponentIDByName(getBuiltinComponentName<Transform>()));
		return entity;
	}

	void EntityManager::Internal::destroyEntity(EntityID entity)
	{
		MEMORY_GUARD;

		for (auto& [ID, owners] : m_componentOwners)
		{
			auto it = owners.find(entity);
			if (it != owners.end())
				owners.remove(it);
		}
		m_entities.remove(m_entities.at(entity));
	}

	std::shared_ptr<ComponentRegistrar> EntityManager::Internal::registrar()
	{
		return m_componentRegistrar;
	}
} // namespace engine