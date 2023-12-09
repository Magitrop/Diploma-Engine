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
	private:
		explicit ComponentID(std::size_t index);

	public:
		// Creates an invalid ComponentID.
		explicit ComponentID();

		std::size_t id() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};
} // namespace engine