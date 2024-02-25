#pragma once

#include <engine/core/math/ray.h>
#include <engine/core/math/vector3.h>

namespace engine
{
	namespace math
	{
		// @param ray: A line to intersect the plane.
		// @param planePoint: Any point on the plane.
		// @param planeNormal: A plane normal.
		Vector3 rayPlaneIntersection(Ray ray, Vector3 planePoint, Vector3 planeNormal);
	} // namespace math_helpers
} // namespace engine