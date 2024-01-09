#pragma once

#include <engine/core/components/component.h>

namespace engine
{
	class ComponentInternal
	{
		// friends
	private:
		friend class EntityManager;

		// members
	protected:
		virtual void attachComponent(ComponentID component) = 0;
		virtual void detachComponent(ComponentID component) = 0;
	};
} // namespace engine