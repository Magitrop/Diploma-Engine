#pragma once

#include <cstddef>
#include <type_traits>

namespace engine
{
	// An ID of an actual Component of some existing Entity.
	// Not to be confused with Unique Component ID that each Component acquires after registration!
	class ComponentID
	{
	public:
		explicit ComponentID(std::size_t id)
			: m_id(id)
		{}

		// Creates an invalid ComponentID.
		explicit ComponentID()
			: m_id(static_cast<std::size_t>(-1))
		{}

		std::size_t id() const { return m_id; }
		operator std::size_t() const { return m_id; }

		bool isValid() const;

	private:
		std::size_t m_id;
	};
} // namespace engine

template <>
struct std::hash<engine::ComponentID>
{
	std::size_t operator()(const engine::ComponentID& key) const
	{
		return key;
	}
};