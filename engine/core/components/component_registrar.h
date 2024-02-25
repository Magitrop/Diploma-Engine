#pragma once

#include <memory>
#include <string>

#include <engine/core/components/component_manager_impl.h>

namespace engine
{
	class ComponentManager;
	class ComponentRegistrar final
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		explicit ComponentRegistrar();

	public:
		~ComponentRegistrar();

		// Should be called by the proper macro (see serialization_helper.h).
		template<typename Component>
		std::size_t registerComponent(Component* manager, std::string componentName);

		template<typename Component>
		void unregisterComponent();

		void unregisterAll();

		[[nodiscard]] ComponentManager* getComponentManager(std::string componentName);
		[[nodiscard]] const ComponentManager* getComponentManager(std::string componentName) const;
		[[nodiscard]] ComponentManager* getComponentManager(std::size_t uniqueComponentID);
		[[nodiscard]] const ComponentManager* getComponentManager(std::size_t uniqueComponentID) const;

		[[nodiscard]] std::size_t getComponentIDByName(std::string componentName) const;

	private:
		std::size_t registerComponentInternal(ComponentManager* manager, std::string componentName);
		void unregisterComponentInternal(std::size_t uniqueComponentID);

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};

	template<typename Component>
	std::size_t ComponentRegistrar::registerComponent(Component* manager, std::string componentName)
	{
		return registerComponentInternal(std::move(manager), std::move(componentName));
	}

	template<typename Component>
	void ComponentRegistrar::unregisterComponent()
	{
		unregisterComponentInternal(Component::getUniqueComponentID());
	}
} // namespace engine