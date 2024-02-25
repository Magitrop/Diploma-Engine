#include "math_helpers.h"

#include <engine/dependencies/glm/glm/ext/quaternion_geometric.hpp>

namespace engine
{
	Vector3 math::rayPlaneIntersection(Ray ray, Vector3 planePoint, Vector3 planeNormal)
	{
		ray.direction.normalize();

		float dot1 = Vector3::dot(planePoint - ray.origin, planeNormal);
		float dot2 = Vector3::dot(ray.direction, planeNormal);
		float distance = dot1 / dot2;

		return ray.origin + ray.direction * distance;
	}
} // namespace engine