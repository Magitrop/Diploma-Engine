#pragma once

#include <engine/core/components/component.h>
#include <engine/core/entity/entity.h>

#include <cstddef>

namespace engine
{
	class ComponentManager
	{
	private:
		friend class EntityManager;
		friend class ComponentRegistrar;

	protected:
		virtual ~ComponentManager() = 0;

		// TODO: should be deleted and replaced by an automatized component initialization
		virtual void attachComponent(ComponentID component) = 0;
		// TODO: should be deleted and replaced by an automatized component finalization
		virtual void detachComponent(ComponentID component) = 0;

		virtual void onComponentAttached(EntityID entity, ComponentID component) {}
		virtual void onComponentDetached(EntityID entity, ComponentID component) {}

	private:
		static std::size_t ID;
	};
} // namespace engine