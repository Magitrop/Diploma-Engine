#include "component.h"

namespace engine
{
	ComponentID::ComponentID(std::size_t id)
		: m_id(id)
	{}

	ComponentID::ComponentID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	std::size_t ComponentID::id() const
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