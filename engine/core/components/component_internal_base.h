#pragma once

#include <string>

#include <engine/core/components/component.h>

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
		virtual void attachComponent(ComponentID component) = 0;
		virtual void detachComponent(ComponentID component) = 0;

		void setComponentName(std::string name);
		void setUniqueComponentID(std::size_t id);

	public:
		std::string getComponentName() const;
		std::size_t getUniqueComponentID() const;

	private:
		std::string m_componentName;
		std::size_t m_uniqueComponentID;
	};
} // namespace engine