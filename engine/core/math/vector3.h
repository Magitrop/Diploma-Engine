#pragma once

#include <limits>

#include <engine/core/math/vector2.h>
#include <engine/dependencies/glm/glm/vec3.hpp>

namespace engine
{
	struct Vector3 : public glm::vec3
	{
		using Base = glm::vec3;
		using Base::vec;
		using Base::operator=;

		Vector3(Base vector) { *this = vector; }
		Vector3(Vector2 vector) 
		{
			this->x = vector.x;
			this->y = vector.y;
		}

		bool isAlmostZero() const;
		float magnitude() const;
		Vector3 normalized() const;
		void normalize();

		bool isValid() const;

		// Component-wise vector summation.
		Vector3& operator += (Vector3 other);
		// Component-wise vector multiplication.
		Vector3& operator *= (Vector3 other);

		static float dot(Vector3 a, Vector3 b);
		static Vector3 cross(Vector3 a, Vector3 b);

		static constexpr Vector3 zero()		{ return Vector3( 0,   0,	0);  }
		static constexpr Vector3 one()		{ return Vector3( 1,   1,	1);  }
		static constexpr Vector3 up()		{ return Vector3( 0,   1,	0);  }
		static constexpr Vector3 down()		{ return Vector3( 0,  -1,	0);  }
		static constexpr Vector3 right()	{ return Vector3( 1,   0,	0);  }
		static constexpr Vector3 left()		{ return Vector3(-1,   0,	0);  }
		static constexpr Vector3 forward()	{ return Vector3( 0,   0,	1);  }
		static constexpr Vector3 backward()	{ return Vector3( 0,   0,  -1);  }
		static constexpr Vector3 invalid()	{ return Vector3( std::numeric_limits<double>::quiet_NaN() );  }
	};
} // namespace engine
