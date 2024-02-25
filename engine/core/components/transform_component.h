#pragma once

#include <engine/core/components/builtin_component.h>
#include <engine/core/math/matrix4x4.h>
#include <engine/core/math/quaternion.h>
#include <engine/core/math/vector3.h>

namespace engine
{
	class Transform final : public BuiltinComponent
	{
	public:
		Transform();
		virtual ~Transform() override;

	public:
		Matrix4x4 modelMatrix(ComponentID id) const;
		Vector3 localPosition(ComponentID id) const;
		Quaternion localRotation(ComponentID id) const;
		Vector3 localScale(ComponentID id) const;

		Vector3 forward(ComponentID id) const;
		Vector3 up(ComponentID id) const;
		Vector3 right(ComponentID id) const;

		/*ComponentID parent(ComponentID id) const;
		void setParent(ComponentID child, ComponentID parent);*/

		void translateLocalBy(ComponentID id, Vector3 value);
		void rotateBy(ComponentID id, Quaternion value);
		void scaleBy(ComponentID id, Vector3 value);
	};
	DEFINE_COMPONENT(Transform)
} // namespace engine