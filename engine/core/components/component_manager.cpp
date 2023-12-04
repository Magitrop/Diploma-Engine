#include "component_manager.h"

namespace engine
{
	ComponentManager::~ComponentManager() {}
	std::size_t ComponentManager::ID = static_cast<std::size_t>(-1);
} // namespace engine