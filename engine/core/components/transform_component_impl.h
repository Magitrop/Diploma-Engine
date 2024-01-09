#pragma once

#include <engine/core/components/component_internal_base.h>
#include <engine/core/components/transform_component.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class TransformInternal final : public ComponentInternal
	{
	public:
		TransformInternal() = default;
		~TransformInternal() = default;

		TransformRef getTransform(ComponentID id);

		virtual void attachComponent(ComponentID component) override;
		virtual void detachComponent(ComponentID component) override;

	private:
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_position;
		PersistentVector<Quaternion, constants::VERY_FREQUENT_COMPONENT> m_rotation;
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_scale;
	};
} // namespace engine