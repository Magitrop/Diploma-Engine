#pragma once

#include <engine/core/math/vector3.h>

namespace engine
{
	struct Ray final
	{
		Vector3 origin;
		Vector3 direction;

		// TODO: static ray functions (intersection() etc.)
	};
} // namespace engine