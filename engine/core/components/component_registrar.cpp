#include "component_registrar.h"

#include <engine/core/components/builtin_component.h>
#include <engine/core/components/component_internal_base.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class ComponentRegistrar::Internal final
	{
	public:
		Internal() = default;
		~Internal() = default;

		std::size_t registerComponent(std::shared_ptr<ComponentManager> manager, std::string componentName);
		void unregisterComponent(std::size_t uniqueComponentID);
		void unregisterAll();

		std::shared_ptr<ComponentManager> getComponentManager(std::string componentName);
		std::shared_ptr<ComponentManager> getComponentManager(std::size_t uniqueComponentID);

		[[nodiscard]] std::size_t getComponentIDByName(std::string componentName) const;

	private:
		PersistentVector<std::shared_ptr<ComponentManager>, 256> m_registeredComponents;
		std::unordered_map<std::string, PageCapacityType> m_nameToID; // Maps component's name to its unique ID.
	};

	std::size_t ComponentRegistrar::Internal::registerComponent(std::shared_ptr<ComponentManager> manager, std::string componentName)
	{
		MEMORY_GUARD;

		if (manager == nullptr)
		{
			ERROR_LOG("Cannot register component. The component manager was null.");
			return static_cast<std::size_t>(-1);
		}

		std::size_t uniqueComponentID = m_registeredComponents.getNextEmptyIndex();
		auto found = m_registeredComponents.find(
			[&uniqueComponentID](const std::shared_ptr<ComponentManager>& entry)
			{
				return entry->getUniqueComponentID() == uniqueComponentID;
			});
		if (found != m_registeredComponents.end())
		{
			WARNING_LOG("Cannot register component. The component was already registered.");
			return found.getIndex();
		}

		m_nameToID[componentName] = uniqueComponentID;
		manager->m_internal->setComponentName(componentName);
		manager->m_internal->setUniqueComponentID(uniqueComponentID);
		return m_registeredComponents.push(std::move(manager)).getIndex();
	}

	void ComponentRegistrar::Internal::unregisterComponent(std::size_t uniqueComponentID)
	{
		auto it = m_registeredComponents.find(
			[&uniqueComponentID](const std::shared_ptr<ComponentManager>& entry)
			{
				return entry->getUniqueComponentID() == uniqueComponentID;
			});
		if (it != m_registeredComponents.end())
		{
			auto& internal = it->get()->m_internal;
			m_nameToID.erase(internal->getComponentName());
			internal->setComponentName(constants::UNREGISTERED_COMPONENT_NAME);
			internal->setUniqueComponentID(constants::UNREGISTERED_UNIQUE_COMPONENT_ID);
			m_registeredComponents.remove(it);
		}
		else
			WARNING_LOG("Cannot unregister component. The component was already unregistered or does not exist.");
	}

	void ComponentRegistrar::Internal::unregisterAll()
	{
		for (auto it = m_registeredComponents.begin(); it != m_registeredComponents.end(); ++it)
		{
			unregisterComponent(it->get()->getUniqueComponentID());
		}
	}

	std::shared_ptr<ComponentManager> ComponentRegistrar::Internal::getComponentManager(std::size_t uniqueComponentID)
	{
		auto it = m_registeredComponents.at(uniqueComponentID);
		if (it != m_registeredComponents.end())
			return it->get();

		ERROR_LOG("Cannot get the manager for this component. The component was not properly registered.");
		return nullptr;
	}

	std::size_t ComponentRegistrar::Internal::getComponentIDByName(std::string componentName) const
	{
		auto found = m_nameToID.find(componentName);
		if (found != m_nameToID.end())
			return found->second;
		return static_cast<std::size_t>(-1);
	}

	std::shared_ptr<ComponentManager> ComponentRegistrar::Internal::getComponentManager(std::string componentName)
	{
		MEMORY_GUARD;

		auto found = m_registeredComponents.find(
			[&componentName](const std::shared_ptr<ComponentManager>& entry)
			{
				return entry->getComponentName() == componentName;
			});
		if (found != m_registeredComponents.end())
			return found->get();

		ERROR_LOG("Cannot get the manager for this component. The component was not properly registered.");
		return nullptr;
	}

	ComponentRegistrar::ComponentRegistrar()
	{
		m_internal = std::make_unique<Internal>();
	}

	ComponentRegistrar::~ComponentRegistrar()
	{}

	std::size_t ComponentRegistrar::registerComponentInternal(std::shared_ptr<ComponentManager> manager, std::string componentName)
	{
		return m_internal->registerComponent(std::move(manager), std::move(componentName));
	}

	void ComponentRegistrar::unregisterComponentInternal(std::size_t uniqueComponentID)
	{
		m_internal->unregisterComponent(uniqueComponentID);
	}

	void ComponentRegistrar::unregisterAll()
	{
		m_internal->unregisterAll();
	}

	std::shared_ptr<ComponentManager> ComponentRegistrar::getComponentManager(std::string componentName)
	{
		return m_internal->getComponentManager(std::move(componentName));
	}

	std::shared_ptr<ComponentManager> ComponentRegistrar::getComponentManager(std::size_t uniqueComponentID)
	{
		return m_internal->getComponentManager(uniqueComponentID);
	}

	std::size_t ComponentRegistrar::getComponentIDByName(std::string componentName) const
	{
		return m_internal->getComponentIDByName(componentName);
	}
} // namespace engine