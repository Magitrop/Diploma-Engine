#pragma once

#include <memory>
#include <string>

#include <engine/core/entity/entity.h>
#include <engine/core/components/component.h>

namespace engine
{
	class ComponentManager;
	class ComponentRegistrar;
	class EntityManager final
	{
	public:
		explicit EntityManager(std::shared_ptr<ComponentRegistrar> registrar);
		~EntityManager();

		// Attaches a Component to an Entity and returns its ID.
		// Does nothing and retuns an invalid ID if the Entity already possesses a Component of that type.
		template<typename Component>
		ComponentID attachComponent(EntityID entity);

		// Detaches a Component from an Entity.
		// Does nothing if the Entity does not possess a Component of that type.
		template<typename Component>
		void detachComponent(EntityID entity);

		// Returns the ID of a Component if the Entity possesses a Component of that type.
		// Otherwise, returns an invalid ID.
		template<typename Component>
		[[nodiscard]] ComponentID getComponent(EntityID entity);

		// Checks whether the Entity possesses a Component of that type.
		// Consider using this function to check for the presence of a Component instead of getComponent.
		template<typename Component>
		[[nodiscard]] bool hasComponent(EntityID entity);

		template<typename Component>
		[[nodiscard]] std::shared_ptr<Component> getComponentManager();

		[[nodiscard]] EntityID createEntity();
		void destroyEntity(EntityID entity);

	private:
		ComponentID attachComponentInternal(EntityID entity, std::string componentName);
		void detachComponentInternal(EntityID entity, std::string componentName);
		ComponentID getComponentInternal(EntityID entity, std::string componentName);
		bool hasComponentInternal(EntityID entity, std::string componentName);
		std::shared_ptr<ComponentManager> getComponentManagerInternal(std::string componentName);

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};

	template<typename Component>
	ComponentID EntityManager::attachComponent(EntityID entity)
	{
		return attachComponentInternal(entity, getBuiltinComponentName<Component>());
	}

	template<typename Component>
	void EntityManager::detachComponent(EntityID entity)
	{
		detachComponentInternal(entity, getBuiltinComponentName<Component>());
	}

	template<typename Component>
	ComponentID EntityManager::getComponent(EntityID entity)
	{
		return getComponentInternal(entity, getBuiltinComponentName<Component>());
	}

	template<typename Component>
	bool EntityManager::hasComponent(EntityID entity)
	{
		return hasComponentInternal(entity, getBuiltinComponentName<Component>());
	}

	template<typename Component>
	std::shared_ptr<Component> EntityManager::getComponentManager()
	{
		return std::dynamic_pointer_cast<Component>(getComponentManagerInternal(getBuiltinComponentName<Component>()));
	}
} // namespace engine