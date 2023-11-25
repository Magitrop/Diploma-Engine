#pragma once

#include <cstddef>

namespace engine
{
	// An ID-only substitution for the actual Entity in ECS.
	// Does not store its own components, but can access them via ObjectsManager.
	class GameObject final
	{
		// friends
	private:
		friend class ObjectsManager;

		// members
	private:
		GameObject(std::size_t index);

	public:


	private:
		const std::size_t m_id;
	};
} // namespace engine