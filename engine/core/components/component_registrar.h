#pragma once

#include <memory>

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

		template<typename Component>
		void registerComponent(std::shared_ptr<ComponentManager> manager);

		template<typename Component>
		void unregisterComponent(std::shared_ptr<ComponentManager> manager);

		void unregisterAll();

		[[nodiscard]] std::size_t getUniqueComponentID(std::shared_ptr<ComponentManager> manager);
		[[nodiscard]] std::shared_ptr<ComponentManager> getComponentManager(std::size_t uniqueComponentID);

	private:
		std::size_t registerComponentInternal(std::shared_ptr<ComponentManager> manager);
		void unregisterComponentInternal(std::shared_ptr<ComponentManager> manager);

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};

	template<typename Component>
	void ComponentRegistrar::registerComponent(std::shared_ptr<ComponentManager> manager)
	{
		Component::ID = registerComponentInternal(manager);
	}

	template<typename Component>
	void ComponentRegistrar::unregisterComponent(std::shared_ptr<ComponentManager> manager)
	{
		unregisterComponentInternal(manager);
		Component::ID = static_cast<std::size_t>(-1);
	}
} // namespace engine