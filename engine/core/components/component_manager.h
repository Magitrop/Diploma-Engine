#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include <engine/core/components/component.h>
#include <engine/core/entity/entity.h>

namespace engine
{
	class ComponentManager
	{
		// friends
	private:
		friend class EntityManager;
		friend class ComponentRegistrar;

		// members
	protected:
		explicit ComponentManager();
		virtual ~ComponentManager() = 0;

		virtual void onComponentAttached(EntityID entity, ComponentID component) {}
		virtual void onComponentDetached(EntityID entity, ComponentID component) {}

	public:
		static std::string getComponentName();
		static std::size_t getUniqueComponentID();

	private:
		class Internal;
		static std::unique_ptr<Internal> m_internal;
	};
} // namespace engine