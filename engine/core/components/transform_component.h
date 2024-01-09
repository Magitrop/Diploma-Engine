#pragma once

#include <engine/core/components/builtin_component.h>
#include <engine/core/math/quaternion.h>
#include <engine/core/math/vector3.h>

namespace engine
{
	struct TransformRef final
	{
		Vector3& position;
		Quaternion& rotation;
		Vector3& scale;
	};

	class Transform final : public BuiltinComponent
	{
	public:
		Transform();
		virtual ~Transform() override;

	public:
		TransformRef getTransform(ComponentID id);
	};
} // namespace engine