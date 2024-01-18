#pragma once

#include <engine/core/math/vector3.h>
#include <engine/dependencies/glm/glm/vec4.hpp>

namespace engine
{
	struct Vector4 : public glm::vec4
	{
		using Base = glm::vec4;
		using Base::vec;
		using Base::operator=;

		Vector4(Base vector) { *this = vector; }
		Vector4(Vector3 vector) 
		{
			this->x = vector.x;
			this->y = vector.y;
			this->z = vector.z;
		}

		static constexpr Vector4 zero()	{ return Vector4(0, 0, 0, 0); }
		static constexpr Vector4 one()	{ return Vector4(1, 1, 1, 1); }
	};
} // namespace engine