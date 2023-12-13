#include "material.h"

namespace engine
{
	Material::~Material()
	{}

	MaterialID::MaterialID(std::size_t index)
		: m_id(index)
	{}

	MaterialID::MaterialID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	std::size_t MaterialID::id() const
	{
		return m_id;
	}

	MaterialID::operator std::size_t() const
	{
		return m_id;
	}

	bool MaterialID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}
} // namespace engine