#pragma once

#include <engine/dependencies/glm/glm/vec2.hpp>

namespace engine
{
	struct Vector2 : public glm::vec2
	{
		using Base = glm::vec2;
		using Base::vec;
		using Base::operator=;

		Vector2(Base vector) { *this = vector; }

		bool isAlmostZero() const;
		float magnitude() const;
		Vector2 normalized() const;
		void normalize();

		static float dot(Vector2 a, Vector2 b);

		static constexpr Vector2 zero()	 { return Vector2( 0,  0); }
		static constexpr Vector2 one()	 { return Vector2( 1,  1); }
		static constexpr Vector2 up()	 { return Vector2( 0,  1); }
		static constexpr Vector2 down()	 { return Vector2( 0, -1); }
		static constexpr Vector2 right() { return Vector2( 1,  0); }
		static constexpr Vector2 left()	 { return Vector2(-1,  0); }
	};
} // namespace engine
