#include "vector3.h"

#include <engine/dependencies/glm/glm/detail/func_geometric.inl>

namespace engine
{
	bool Vector3::isAlmostZero() const
	{
		static const float epsilon = 1e-5;
		return magnitude() < epsilon;
	}

	float Vector3::magnitude() const
	{
		return glm::length(static_cast<Base>(*this));
	}

	Vector3 Vector3::normalized() const
	{
		return glm::normalize(*this);
	}

	void Vector3::normalize()
	{
		*this = glm::normalize(*this);
	}

	bool Vector3::isValid() const
	{
		return !isnan(x) && !isnan(y) && !isnan(z);
	}

	Vector3& Vector3::operator += (Vector3 other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& Vector3::operator *= (Vector3 other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	float Vector3::dot(Vector3 a, Vector3 b)
	{
		return glm::dot(static_cast<Base>(a), static_cast<Base>(b));
	}

	Vector3 Vector3::cross(Vector3 a, Vector3 b)
	{
		return glm::cross(static_cast<Base>(a), static_cast<Base>(b));
	}
} // namespace engine