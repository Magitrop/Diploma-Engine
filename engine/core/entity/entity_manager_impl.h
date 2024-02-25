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
		template<typename T>
		using DataVector = PersistentVector<T, constants::COMPONENT_OWNERS_PER_PAGE>;

		Internal(std::shared_ptr<ComponentRegistrar> registrar);
		~Internal() = default;

		ComponentID attachComponent(EntityID entity, std::size_t uniqueComponentID);
		void detachComponent(EntityID entity, std::size_t uniqueComponentID);
		ComponentID getComponent(EntityID entity, std::size_t uniqueComponentID);
		bool hasComponent(EntityID entity, std::size_t uniqueComponentID);
		ComponentManager* getComponentManager(std::size_t uniqueComponentID);
		const ComponentManager* getComponentManager(std::size_t uniqueComponentID) const;

		EntityID createEntity();
		void destroyEntity(EntityID entity);

		std::shared_ptr<ComponentRegistrar> registrar();

		const DataVector<std::string>& entityNames() const { return m_entityName; }

	private:
		std::unordered_map<std::size_t, DataVector<EntityID>> m_componentOwners;

		struct EntityWrapper
		{
			// TODO: provide an opportunity to (dynamically?) extend the capacity of the bitset
			// The extension (copying from a narrower one and pasting into a wider one) must be performed via the bitset stream operators << and >>
			// Also, user must be informed about that extension and required to accept it - otherwise further creation of components must be forbidden
			std::bitset<256> components;
		};
		DataVector<EntityWrapper> m_entities;
		DataVector<std::string> m_entityName;

		std::shared_ptr<ComponentRegistrar> m_componentRegistrar;
	};
} // namespace engine