#pragma once

#include <engine/core/components/component_manager.h>
#include <engine/internal/core/constants/runtime_constants.h>

namespace engine
{
	class ComponentManager::Internal final
	{
	public:
		std::string getComponentName();
		void setComponentName(std::string name);

		std::size_t getUniqueComponentID();
		void setUniqueComponentID(std::size_t id);

	private:
		std::string m_componentName = constants::UNREGISTERED_COMPONENT_NAME;
		std::size_t m_uniqueComponentID = static_cast<std::size_t>(-1);
	};
} // namespace engine