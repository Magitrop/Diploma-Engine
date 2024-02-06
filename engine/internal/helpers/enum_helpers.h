#pragma once

#include <engine/editor/gizmo/gizmo.h>

template<typename Enum>
static constexpr Enum operator | (Enum lhs, Enum rhs)
{
	std::size_t result = static_cast<std::size_t>(lhs) | static_cast<std::size_t>(rhs);
	return static_cast<Enum>(result);
}

template<typename Enum>
static constexpr Enum operator & (Enum lhs, Enum rhs)
{
	std::size_t result = static_cast<std::size_t>(lhs) & static_cast<std::size_t>(rhs);
	return static_cast<Enum>(result);
}