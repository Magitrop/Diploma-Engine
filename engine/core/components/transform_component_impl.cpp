#include "transform_component_impl.h"

namespace engine
{
	TransformRef TransformInternal::getTransform(ComponentID id)
	{
		return { m_position.at(id)->get(), m_rotation.at(id)->get(), m_scale.at(id)->get() };
	}

	void TransformInternal::attachComponent(ComponentID component) /* override */
	{
		m_position.push(Vector3(0, 0, 0));
		m_rotation.push(Quaternion({ 0, 0, 0 }));
		m_scale.push(Vector3(1, 1, 1));
	}

	void TransformInternal::detachComponent(ComponentID component) /* override */
	{
		m_position.remove(m_position.begin() + component.id());
		m_rotation.remove(m_rotation.begin() + component.id());
		m_scale.remove(m_scale.begin() + component.id());
	}
} // namespace engine