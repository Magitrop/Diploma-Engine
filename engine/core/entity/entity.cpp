#include "entity.h"

namespace engine
{
	EntityID::EntityID(std::size_t id /* = static_cast<std::size_t>(-1) */)
		: m_id(id)
	{}

	std::size_t EntityID::getID() const
	{
		return m_id;
	}

	EntityID::operator std::size_t() const
	{
		return m_id;
	}

	bool EntityID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}
} // namespace engine