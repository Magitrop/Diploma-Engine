#include "transform_component.h"

#include <engine/core/components/transform_component_impl.h>

namespace engine
{
	Transform::Transform()
	{
		m_internal = std::shared_ptr<TransformInternal>(new TransformInternal());
	}

	Transform::~Transform()
	{}

	Matrix4x4 Transform::modelMatrix(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->modelMatrix(id);
	}

	Vector3 Transform::localPosition(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->localPosition(id);
	}

	Quaternion Transform::localRotation(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->localRotation(id);
	}

	Vector3 Transform::localScale(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->localScale(id);
	}

	Vector3 Transform::forward(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->forward(id);
	}

	Vector3 Transform::up(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->up(id);
	}

	Vector3 Transform::right(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->right(id);
	}

	/*ComponentID Transform::parent(ComponentID id) const
	{
		return static_cast<TransformInternal*>(m_internal.get())->parent(id);
	}

	void Transform::setParent(ComponentID child, ComponentID parent)
	{
		return static_cast<TransformInternal*>(m_internal.get())->setParent(child, parent);
	}*/

	void Transform::translateLocalBy(ComponentID id, Vector3 value)
	{
		static_cast<TransformInternal*>(m_internal.get())->translateLocalBy(id, value);
	}

	void Transform::rotateBy(ComponentID id, Quaternion value)
	{
		static_cast<TransformInternal*>(m_internal.get())->rotateBy(id, value);
	}

	void Transform::scaleBy(ComponentID id, Vector3 value)
	{

	}
} // namespace engine