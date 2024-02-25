#include "entity.h"

namespace engine
{
	bool EntityID::isValid() const
	{
		return m_id != static_cast<std::uint32_t>(-1);
	}
} // namespace engine