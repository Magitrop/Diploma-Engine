#pragma once

#include <string>

#include <engine/core/components/component.h>
#include <engine/core/entity/entity.h>

namespace engine
{
	class ComponentInternal
	{
		// friends
	private:
		friend class EntityManager;
		friend class ComponentRegistrar;

		// members
	protected:
		virtual void attachComponent(EntityID entity, ComponentID component) = 0;
		virtual void detachComponent(EntityID entity, ComponentID component) = 0;
	};
} // namespace engine