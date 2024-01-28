#pragma once

#include <engine/core/components/component_manager.h>

#define DEFINE_COMPONENT_HEADER \
template<typename Comp> \
static constexpr const char* getBuiltinComponentName() \
{ \
	return "unknown"; \
}

#define DEFINE_COMPONENT(comp) \
template<> \
static constexpr const char* getBuiltinComponentName<comp>() \
{ \
	return #comp; \
}

namespace engine
{
	DEFINE_COMPONENT_HEADER

	class ComponentInternal;
	class BuiltinComponent : public ComponentManager
	{
		// friends
	private:
		friend class EntityManager;
		friend class ComponentRegistrar;

		// members
	public:
		std::string getComponentName() const;
		std::size_t getUniqueComponentID() const;

	protected:
		std::shared_ptr<ComponentInternal> m_internal;
	};
} // namespace engine