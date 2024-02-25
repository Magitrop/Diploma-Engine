#pragma once

#include <engine/core/entity/entity_manager.h>

namespace engine
{
	class EntityManager;
	class EntityManagerAccessor final
	{
	public:
		explicit EntityManagerAccessor(EntityManager* entityManager);

		EntityManager* entityManager() { return m_entityManager; }
		EntityManager::Internal* entityManagerInternal() { return m_entityManager->m_internal.get(); }

	private:
		EntityManager* m_entityManager;
	};
} // namespace engine
