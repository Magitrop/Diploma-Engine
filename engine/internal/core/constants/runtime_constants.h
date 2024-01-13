#pragma once

#include <cstddef>

namespace engine
{
	namespace constants
	{
		constexpr const char* UNREGISTERED_COMPONENT_NAME = "unknown"; // The name for all unregistered and unknown components.
		constexpr std::size_t UNREGISTERED_UNIQUE_COMPONENT_ID = static_cast<std::size_t>(-1); // The unique ID for all unregistered and unknown components.

		constexpr std::size_t VERY_RARE_COMPONENT = 8; // The amount of entities per page in data vectors of the very rare components.
		constexpr std::size_t RARE_COMPONENT = 32; // The amount of entities per page in data vectors of the rare components.
		constexpr std::size_t REGULAR_COMPONENT = 128; // The amount of entities per page in data vectors of the regular components.
		constexpr std::size_t FREQUENT_COMPONENT = 512; // The amount of entities per page in data vectors of the frequent components.
		constexpr std::size_t VERY_FREQUENT_COMPONENT = 2048; // The amount of entities per page in data vectors of the very frequent components.

		constexpr std::size_t COMPONENT_OWNERS_PER_PAGE = 2048; // The amount of component owners per page in the Entity Manager.

		constexpr std::size_t SHADERS_PER_PAGE = 128; // The amount of shaders per page in the Resource Manager.
		constexpr std::size_t MATERIALS_PER_PAGE = 256; // The amount of materials per page in the Resource Manager.
		constexpr std::size_t MESHES_PER_PAGE = 256; // The amount of meshes per page in the Resource Manager.

		constexpr std::size_t RENDER_CONTEXTS_PER_PAGE = 8; // The amount of render context data per page in the Render Pipeline.
	}
}