#include "entity_selection.h"

#include <unordered_set>

template <>
struct std::hash<engine::EntityID>
{
	std::size_t operator()(const engine::EntityID& key) const
	{
		return key;
	}
};

namespace engine
{
	class EntitySelection::Internal final
	{
	public:
		Internal() = default;
		~Internal() = default;

		const SelectedEntities& selection() const;

		void select(EntityID entity);
		void deselect(EntityID entity);
		void deselectAll();

		bool isSelected(EntityID entity);

	private:
		std::vector<EntityID> m_selectedEntities;
		std::unordered_set<EntityID> m_selectedEntitiesHelper;
	};

	const EntitySelection::SelectedEntities& EntitySelection::Internal::selection() const
	{
		return m_selectedEntities;
	}

	void EntitySelection::Internal::select(EntityID entity)
	{
		if (!isSelected(entity))
		{
			m_selectedEntitiesHelper.insert(entity);
			m_selectedEntities.push_back(entity);
		}
	}

	void EntitySelection::Internal::deselect(EntityID entity)
	{
		if (isSelected(entity))
		{
			m_selectedEntitiesHelper.erase(entity);

			auto found = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
			if (found != m_selectedEntities.end())
				m_selectedEntities.erase(found);
		}
	}

	void EntitySelection::Internal::deselectAll()
	{
		m_selectedEntitiesHelper.clear();
		m_selectedEntities.clear();
	}

	bool EntitySelection::Internal::isSelected(EntityID entity)
	{
		return m_selectedEntitiesHelper.find(entity) != m_selectedEntitiesHelper.end();
	}

	EntitySelection::EntitySelection()
	{
		m_internal = std::make_unique<Internal>();
	}

	EntitySelection::~EntitySelection()
	{}

	const EntitySelection::SelectedEntities& EntitySelection::selection() const
	{
		return m_internal->selection();
	}

	void EntitySelection::select(EntityID entity)
	{
		m_internal->select(entity);
	}

	void EntitySelection::deselect(EntityID entity)
	{
		m_internal->deselect(entity);
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
