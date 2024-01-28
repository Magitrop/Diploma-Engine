#pragma once

#include <engine/core/components/builtin_component.h>
#include <engine/core/math/matrix4x4.h>
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
		//TransformRef getTransform(ComponentID id);
		Matrix4x4& modelMatrix(ComponentID id);
	};
	DEFINE_COMPONENT(Transform)
} // namespace engine