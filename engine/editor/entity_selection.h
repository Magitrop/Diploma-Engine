#pragma once

#include <memory>
#include <vector>

#include <engine/core/entity/entity.h>
#include <engine/internal/helpers/dependencies.h>

namespace engine
{
	class GizmoManager;
	class EntityManager;
	class EntitySelection final : public Dependencies<EntityManager, GizmoManager>
	{
	public:
		explicit EntitySelection(DependencyContext dependencies);
		~EntitySelection();

		using SelectedEntities = std::vector<EntityID>;

		const SelectedEntities& selection() const;

		// Returns true if successfully selected an entity.
		bool select(EntityID entity);
		// Returns true if successfully deselected an entity.
		bool deselect(EntityID entity);
		void deselectAll();

		bool isSelected(EntityID entity) const;

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine
