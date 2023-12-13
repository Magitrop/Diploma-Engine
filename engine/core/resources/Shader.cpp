#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace engine
{
	Shader::~Shader()
	{}

	ShaderID::ShaderID(std::size_t index)
		: m_id(index)
	{}

	ShaderID::ShaderID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	std::size_t ShaderID::id() const
	{
		return m_id;
	}

	ShaderID::operator std::size_t() const
	{
		return m_id;
	}

	bool ShaderID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}
} // namespace engine