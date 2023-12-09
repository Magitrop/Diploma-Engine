#pragma once

#include <cstddef>

namespace engine
{
	namespace constants
	{
		constexpr std::size_t VERY_RARE_COMPONENT = 8; // The amount of entities per page in data vectors of the very rare components.
		constexpr std::size_t RARE_COMPONENT = 32; // The amount of entities per page in data vectors of the rare components.
		constexpr std::size_t REGULAR_COMPONENT = 128; // The amount of entities per page in data vectors of the regular components.
		constexpr std::size_t FREQUENT_COMPONENT = 512; // The amount of entities per page in data vectors of the frequent components.
		constexpr std::size_t VERY_FREQUENT_COMPONENT = 2048; // The amount of entities per page in data vectors of the very frequent components.
	}
}