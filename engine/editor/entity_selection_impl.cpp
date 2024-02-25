#include "entity_selection_impl.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/editor/gizmo/gizmo_manager.h>

#include <engine/internal/helpers/assert.h>

namespace engine
{
	EntitySelection::Internal::Internal(DependencyContext dependencies)
		: Dependencies(dependencies)
	{}

	const EntitySelection::SelectedEntities& EntitySelection::Internal::selection() const
	{
		return m_selectedEntities;
	}

	bool EntitySelection::Internal::select(EntityID entity)
	{
		if (!isSelected(entity) && entity != EntityID())
		{
			EntityManager* entityManager = getDependency<EntityManager>();
			if (!entityManager->hasComponent<Transform>(entity))
				return false;

			m_selectedEntitiesHelper.insert(entity);
			m_selectedEntities.push_back(entity);

			// TODO: gizmo with multiple selection
			getDependency<GizmoManager>()->attach(entityManager->getComponent<Transform>(entity));
			return true;
		}
		return false;
	}

	bool EntitySelection::Internal::deselect(EntityID entity)
	{
		if (isSelected(entity))
		{
			m_selectedEntitiesHelper.erase(entity);

			auto found = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
			if (found != m_selectedEntities.end())
			{
				m_selectedEntities.erase(found);
				return true;
			}
		}
		return false;
	}

	void EntitySelection::Internal::deselectAll()
	{
		m_selectedEntitiesHelper.clear();
		m_selectedEntities.clear();
		getDependency<GizmoManager>()->detach();
	}

	bool EntitySelection::Internal::isSelected(EntityID entity)
	{
		return m_selectedEntitiesHelper.find(entity) != m_selectedEntitiesHelper.end();
	}
} // namespace engine