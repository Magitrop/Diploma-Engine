#pragma once

#include <cstddef>

namespace engine
{
	// An ID-only substitution for the actual Entity in ECS.
	// Does not store its own Components. Use engine::EntityManager to access them instead.
	class EntityID final
	{
		// friends
	private:
		friend class EntityManager;

		// members
	public:
		EntityID(std::size_t index = static_cast<std::size_t>(-1));

		std::size_t getID() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};
} // namespace engine