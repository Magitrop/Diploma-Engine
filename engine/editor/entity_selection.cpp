#include "entity_selection.h"

#include <engine/editor/entity_selection_impl.h>

namespace engine
{
	EntitySelection::EntitySelection(DependencyContext dependencies)
		: Dependencies(dependencies)
	{
		m_internal = std::make_unique<Internal>(dependencies);
	}

	EntitySelection::~EntitySelection()
	{}

	const EntitySelection::SelectedEntities& EntitySelection::selection() const
	{
		return m_internal->selection();
	}

	bool EntitySelection::select(EntityID entity)
	{
		return m_internal->select(entity);
	}

	bool EntitySelection::deselect(EntityID entity)
	{
		return m_internal->deselect(entity);
	}

	void EntitySelection::deselectAll()
	{
		m_internal->deselectAll();
	}

	bool EntitySelection::isSelected(EntityID entity) const
	{
		return m_internal->isSelected(entity);
	}
} // namespace engine
