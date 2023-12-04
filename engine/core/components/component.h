#pragma once

#include <cstddef>

namespace engine
{
	// An ID of an actual Component of some existing Entity.
	// Not to be confused with Unique Component ID that each Component acquires after registration!
	class ComponentID
	{
		// friends
	private:
		friend class EntityManager;

		// members
	public:
		ComponentID(std::size_t index = static_cast<std::size_t>(-1));

		std::size_t getID() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};
} // namespace engine