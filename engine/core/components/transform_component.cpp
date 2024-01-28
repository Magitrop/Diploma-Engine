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

	/*TransformRef Transform::getTransform(ComponentID id)
	{
		return std::static_pointer_cast<TransformInternal>(m_internal)->getTransform(id);
	}*/

	Matrix4x4& Transform::modelMatrix(ComponentID id)
	{
		return std::static_pointer_cast<TransformInternal>(m_internal)->modelMatrix(id);
	}
} // namespace engine