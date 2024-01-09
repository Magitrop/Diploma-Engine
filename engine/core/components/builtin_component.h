#pragma once

#include <engine/core/components/component_manager.h>

namespace engine
{
	class ComponentInternal;
	class BuiltinComponent : public ComponentManager
	{
		// friends
	private:
		friend class EntityManager;

		// members
	protected:
		std::shared_ptr<ComponentInternal> m_internal;
	};
} // namespace engine