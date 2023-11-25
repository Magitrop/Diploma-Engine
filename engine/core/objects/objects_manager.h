#pragma once

#include <engine/internal/helpers/persistent_vector.h>
#include <engine/core/objects/game_object.h>

namespace engine
{
	class ObjectsManager final
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		ObjectsManager() = default;

	public:


	private:
		struct GameObject
		{
			std::vector<std::size_t> m_components;
		};

		PersistentVector<GameObject> m_objects;
	};
} // namespace engine