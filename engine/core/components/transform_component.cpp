#include "transform_component.h"

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class TransformComponent::Internal final
	{
	public:
		Internal() = default;
		~Internal() = default;

		TransformRef getTransform(ComponentID id);

		void attachComponent(ComponentID component);
		void detachComponent(ComponentID component);

	private:
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_position;
		PersistentVector<Quaternion, constants::VERY_FREQUENT_COMPONENT> m_rotation;
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_scale;
	};

	TransformRef TransformComponent::Internal::getTransform(ComponentID id)
	{
		return { m_position.at(id)->get(), m_rotation.at(id)->get(), m_scale.at(id)->get() };
	}

	void TransformComponent::Internal::attachComponent(ComponentID component)
	{
		m_position.push(Vector3(0, 0, 0));
		m_rotation.push(Quaternion({ 0, 0, 0 }));
		m_scale.push(Vector3(1, 1, 1));
	}

	void TransformComponent::Internal::detachComponent(ComponentID component)
	{
		m_position.remove(m_position.begin() + component.id());
		m_rotation.remove(m_rotation.begin() + component.id());
		m_scale.remove(m_scale.begin() + component.id());
	}

	TransformComponent::TransformComponent()
	{
		m_internal = std::make_unique<Internal>();
	}

	TransformComponent::~TransformComponent()
	{}

	TransformRef TransformComponent::getTransform(ComponentID id)
	{
		return m_internal->getTransform(id);
	}

	void TransformComponent::attachComponent(ComponentID component)
	{
		m_internal->attachComponent(component);
	}

	void TransformComponent::detachComponent(ComponentID component)
	{
		m_internal->detachComponent(component);
	}
} // namespace engine