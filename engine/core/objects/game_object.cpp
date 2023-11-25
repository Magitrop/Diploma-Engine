#include "game_object.h"
#include <vector>
#include <unordered_map>

namespace engine
{
	GameObject::GameObject(std::size_t id)
		: m_id(id)
	{}
} // namespace engine