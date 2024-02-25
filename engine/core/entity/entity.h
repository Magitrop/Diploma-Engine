#pragma once

#include <cstdint>
#include <type_traits>

namespace engine
{
	// An ID-only substitution for the actual Entity in ECS.
	// Does not store its own Components. Use EntityManager to access them instead.
	class EntityID final
	{
	public:
		explicit constexpr EntityID(std::uint32_t id)
			: m_id(id)
		{}

		// Creates an invalid EntityID.
		explicit constexpr EntityID()
			: m_id(static_cast<std::uint32_t>(-1))
		{}

		std::uint32_t id() const { return m_id; }
		operator std::uint32_t() const { return m_id; }

		bool isValid() const;

	private:
		std::uint32_t m_id;
	};
} // namespace engine

template <>
struct std::hash<engine::EntityID>
{
	std::size_t operator()(const engine::EntityID& key) const
	{
		return key;
	}
};