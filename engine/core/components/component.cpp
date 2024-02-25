#include "component.h"

namespace engine
{
	bool ComponentID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}
} // namespace engine