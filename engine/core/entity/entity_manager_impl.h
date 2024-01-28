#pragma once

#include <bitset>
#include <unordered_map>

#include <engine/core/entity/entity_manager.h>
#include <engine/internal/core/constants/runtime_constants.h>
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

		std::shared_ptr<ComponentRegistrar> getRegistrar();

	private:
		std::unordered_map<std::size_t, PersistentVector<EntityID, constants::COMPONENT_OWNERS_PER_PAGE>> m_componentOwners;

		struct EntityWrapper
		{
			// TODO: provide an opportunity to (dynamically?) extend the capacity of the bitset
			// Extension (copying from a narrower one and pasting into a wider one) must be performed via the bitset stream operators << and >>
			// Also, user must be informed about that extension and made required to accept it - otherwise further creation of components must be forbidden
			std::bitset<256> components;
		};
		PersistentVector<EntityWrapper, constants::COMPONENT_OWNERS_PER_PAGE> m_entities;

		std::shared_ptr<ComponentRegistrar> m_componentRegistrar;
	};
} // namespace engine