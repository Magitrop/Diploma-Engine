#pragma once

#include <cstddef>

namespace engine
{
	// An ID-only substitution for the actual Entity in ECS.
	// Does not store its own Components. Use EntityManager to access them instead.
	class EntityID final
	{
	public:
		explicit EntityID(std::size_t index);

		// Creates an invalid EntityID.
		explicit EntityID();

		std::size_t id() const;
		operator std::size_t() const;


		bool isValid() const;

	private:
		std::size_t m_id;
	};
} // namespace engine