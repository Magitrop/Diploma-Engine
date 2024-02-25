#pragma once

#include <unordered_set>

#include <engine/editor/entity_selection.h>

namespace engine
{
	class EntitySelection::Internal final : public Dependencies<EntityManager, GizmoManager>
	{
	public:
		Internal(DependencyContext dependencies);
		~Internal() = default;

		const SelectedEntities& selection() const;

		bool select(EntityID entity);
		bool deselect(EntityID entity);
		void deselectAll();

		bool isSelected(EntityID entity);

	private:
		std::vector<EntityID> m_selectedEntities;
		std::unordered_set<EntityID> m_selectedEntitiesHelper;
	};
} // namespace engine