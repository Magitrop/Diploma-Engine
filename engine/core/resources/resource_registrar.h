#pragma once

#include <engine/core/resources/resource_manager.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class ResourceRegistrar : public ResourceManager
	{
	public:
		virtual void registerBuiltinShaders() = 0;
		virtual void registerBuiltinMaterials() = 0;
	};
} // namespace engine