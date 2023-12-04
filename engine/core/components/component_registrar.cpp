#include "component_registrar.h"

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class ComponentRegistrar::Internal final
	{
	public:
		Internal() = default;
		~Internal() = default;

		std::size_t registerComponent(std::shared_ptr<ComponentManager> manager);
		void unregisterComponent(std::shared_ptr<ComponentManager> manager);
		void unregisterAll();

		std::size_t getUniqueComponentID(std::shared_ptr<ComponentManager> manager);
		std::shared_ptr<ComponentManager> getComponentManager(std::size_t uniqueComponentID);

	private:
		PersistentVector<std::shared_ptr<ComponentManager>, 256> m_registeredComponents;
	};

	std::size_t ComponentRegistrar::Internal::registerComponent(std::shared_ptr<ComponentManager> manager)
	{
		if (manager == nullptr)
		{
			ERROR_LOG("Cannot register component. The component manager was null.");
			return static_cast<std::size_t>(-1);
		}

		auto found = m_registeredComponents.find(manager);
		if (found != m_registeredComponents.end())
		{
			WARNING_LOG("Cannot register component. The component was already registered.");
			return found.getIndex();
		}

		std::size_t index = static_cast<std::size_t>(-1);
		m_registeredComponents.push(std::move(manager), &index);
		return index;
	}

	void ComponentRegistrar::Internal::unregisterComponent(std::shared_ptr<ComponentManager> manager)
	{
		if (manager == nullptr)
		{
			ERROR_LOG("Cannot register component. The component manager was null.");
			return;
		}

		auto it = m_registeredComponents.find(manager);
		if (it != m_registeredComponents.end())
			m_registeredComponents.remove(it);
		else
			WARNING_LOG("Cannot unregister component. The component was already unregistered or does not exist.");
	}

	void ComponentRegistrar::Internal::unregisterAll()
	{
		m_registeredComponents.clear();
	}

	std::size_t ComponentRegistrar::Internal::getUniqueComponentID(std::shared_ptr<ComponentManager> manager)
	{
		auto it = m_registeredComponents.find(manager);
		if (it != m_registeredComponents.end())
			return it.getIndex();

		ERROR_LOG("Cannot get unique component ID. The component was not properly registered.");
		return static_cast<std::size_t>(-1);
	}

	std::shared_ptr<ComponentManager> ComponentRegistrar::Internal::getComponentManager(std::size_t uniqueComponentID)
	{
		auto it = m_registeredComponents.at(uniqueComponentID);
		if (it != m_registeredComponents.end())
			return *it;

		ERROR_LOG("Cannot get the manager for this component. The component was not properly registered.");
		return nullptr;
	}

	ComponentRegistrar::ComponentRegistrar()
	{
		m_internal = std::make_unique<Internal>();
	}

	ComponentRegistrar::~ComponentRegistrar()
	{}

	std::size_t ComponentRegistrar::registerComponentInternal(std::shared_ptr<ComponentManager> manager)
	{
		return m_internal->registerComponent(std::move(manager));
	}

	void ComponentRegistrar::unregisterComponentInternal(std::shared_ptr<ComponentManager> manager)
	{
		m_internal->unregisterComponent(std::move(manager));
	}

	void ComponentRegistrar::unregisterAll()
	{
		m_internal->unregisterAll();
	}

	std::size_t ComponentRegistrar::getUniqueComponentID(std::shared_ptr<ComponentManager> manager)
	{
		return m_internal->getUniqueComponentID(std::move(manager));
	}

	std::shared_ptr<ComponentManager> ComponentRegistrar::getComponentManager(std::size_t id)
	{
		return m_internal->getComponentManager(id);
	}
} // namespace engine