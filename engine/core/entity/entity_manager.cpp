#include "entity_manager.h"

#include <bitset>
#include <unordered_map>

#include <engine/core/components/component_manager.h>
#include <engine/core/components/component_registrar.h>
#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class EntityManager::Internal final
	{
	public:
		Internal(std::shared_ptr<ComponentRegistrar> registrar);
		~Internal() = default;

		ComponentID attachComponent(EntityID entity, std::size_t uniqueComponentID);
		void detachComponent(EntityID entity, std::size_t uniqueComponentID);
		ComponentID getComponent(EntityID entity, std::size_t uniqueComponentID);
		bool hasComponent(EntityID entity, std::size_t uniqueComponentID);
		std::shared_ptr<ComponentManager> getComponentManager(std::size_t uniqueComponentID);

		EntityID createEntity();
		void destroyEntity(EntityID entity);

	private:
		void validateComponentManager(std::size_t uniqueComponentID);

		struct ComponentVector
		{
			std::shared_ptr<ComponentManager> manager = nullptr;
			PersistentVector<EntityID, 2048> componentOwners;
		};
		std::unordered_map<std::size_t, ComponentVector> m_componentVectors;

		struct EntityWrapper
		{
			// TODO: provide an opportunity to (dynamically?) extend the capacity of the bitset
			// Extension (copying from a narrower one and pasting into a wider one) must be performed via the bitset stream operators << and >>
			// Also, user must be informed about that extension and made required to accept it - otherwise further creation of components must be forbidden
			std::bitset<256> components;
		};
		PersistentVector<EntityWrapper, 2048> m_entities;

		std::shared_ptr<ComponentRegistrar> m_componentRegistrar;
	};

	EntityManager::Internal::Internal(std::shared_ptr<ComponentRegistrar> registrar)
		: m_componentRegistrar(registrar)
	{}

	ComponentID EntityManager::Internal::attachComponent(EntityID entity, std::size_t uniqueComponentID)
	{
		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot attach component. The component was not properly registered.");
			return ComponentID(static_cast<std::size_t>(-1));
		}

		auto& entityComponents = m_entities.at(entity)->get().components;
		auto& componentOwners = m_componentVectors[uniqueComponentID].componentOwners;

		// already attached
		if (entityComponents[uniqueComponentID])
		{
			WARNING_LOG("Cannot attach component. This entity already have one. To check if an entity possesses some component use hasComponent function.");
			return ComponentID(static_cast<std::size_t>(-1));
		}

		ComponentID result = ComponentID(componentOwners.push(entity).getIndex());
		entityComponents[uniqueComponentID] = 1;

		getComponentManager(uniqueComponentID)->attachComponent(result);
		getComponentManager(uniqueComponentID)->onComponentAttached(entity, result);

		return result;
	}

	void EntityManager::Internal::detachComponent(EntityID entity, std::size_t uniqueComponentID)
	{
		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot detach component. The component was not properly registered.");
			return;
		}

		auto& entityComponents = m_entities.at(entity)->get().components;
		auto& componentOwners = m_componentVectors[uniqueComponentID].componentOwners;

		// already detached
		if (!entityComponents[uniqueComponentID])
		{
			WARNING_LOG("Cannot detach component. This entity does not have one. To check if an entity possesses some component use hasComponent function.");
			return;
		}

		auto owner = componentOwners.find(entity);

		ComponentID component = ComponentID(owner.getIndex());
		getComponentManager(uniqueComponentID)->onComponentDetached(entity, component);
		getComponentManager(uniqueComponentID)->detachComponent(component);

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

		const auto& componentOwners = m_componentVectors[uniqueComponentID].componentOwners;

		std::size_t index = 0;
		for (const auto& owner : componentOwners)
		{
			if (owner.get() == entity)
				return ComponentID(index);
			++index;
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
		if (uniqueComponentID == static_cast<std::size_t>(-1))
		{
			ERROR_LOG("Cannot get the manager for this component. The component was not properly registered.");
			return nullptr;
		}

		validateComponentManager(uniqueComponentID);
		return m_componentVectors[uniqueComponentID].manager;
	}

	EntityID EntityManager::Internal::createEntity()
	{
		return EntityID(m_entities.push(EntityWrapper()).getIndex());
	}

	void EntityManager::Internal::destroyEntity(EntityID entity)
	{
		for (auto& [ID, entities] : m_componentVectors)
		{
			auto& owners = entities.componentOwners;
			auto it = owners.find(entity);
			if (it != owners.end())
				owners.remove(it);
		}
		m_entities.remove(m_entities.at(entity));
	}

	void EntityManager::Internal::validateComponentManager(std::size_t uniqueComponentID)
	{
		std::shared_ptr<ComponentManager>& manager = m_componentVectors[uniqueComponentID].manager;
		if (manager == nullptr)
		{
			manager = m_componentRegistrar->getComponentManager(uniqueComponentID);
		}
	}

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