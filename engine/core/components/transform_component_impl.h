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

		//TransformRef getTransform(ComponentID id);

		virtual void attachComponent(ComponentID component) override;
		virtual void detachComponent(ComponentID component) override;

		Matrix4x4& modelMatrix(ComponentID id);

	private:
		PersistentVector<Matrix4x4, constants::VERY_FREQUENT_COMPONENT> m_modelMatrix;
	};
} // namespace engine