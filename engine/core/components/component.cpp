#include "component.h"

namespace engine
{
	ComponentID::ComponentID(std::size_t id /* = static_cast<std::size_t>(-1) */)
		: m_id(id)
	{}

	std::size_t ComponentID::getID() const
	{
		return m_id;
	}

	ComponentID::operator std::size_t() const
	{
		return m_id;
	}

	bool ComponentID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}
} // namespace engine