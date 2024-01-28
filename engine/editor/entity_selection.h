#pragma once

#include <memory>
#include <vector>

#include <engine/core/entity/entity.h>

namespace engine
{
	class EntitySelection final
	{
	public:
		explicit EntitySelection();
		~EntitySelection();

		using SelectedEntities = std::vector<EntityID>;

		const SelectedEntities& selection() const;

		void select(EntityID entity);
		void deselect(EntityID entity);
		void deselectAll();

		bool isSelected(EntityID entity) const;

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine
