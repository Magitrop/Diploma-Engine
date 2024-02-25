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

		virtual void attachComponent(EntityID entity, ComponentID component) override;
		virtual void detachComponent(EntityID entity, ComponentID component) override;

		Matrix4x4 modelMatrix(ComponentID id) const;

		Vector3 position(ComponentID id) const;
		Vector3 localPosition(ComponentID id) const;

		Quaternion rotation(ComponentID id) const;
		Quaternion localRotation(ComponentID id) const;

		Vector3 scale(ComponentID id) const;
		Vector3 localScale(ComponentID id) const;

		Vector3 forward(ComponentID id) const;
		Vector3 up(ComponentID id) const;
		Vector3 right(ComponentID id) const;

		void translateLocalBy(ComponentID id, Vector3 value);
		void rotateBy(ComponentID id, Quaternion value);
		void scaleBy(ComponentID id, Vector3 value);

	private:
		void updateRelativeDirections(ComponentID id);
		void updateModelMatrix(ComponentID id);

#pragma region Parent System (WIP)
		ComponentID parent(ComponentID id) const;
		void setParent(ComponentID child, ComponentID parent);

		void updateChildren(ComponentID id);
		void updatePosition(ComponentID child, ComponentID parent);
		void updateRotation(ComponentID child, ComponentID parent);
		void updateScale(ComponentID child, ComponentID parent);

		// Checks if there is a circular parent dependency for an entity, since two entities cannot be each other's direct or indirect parents.
		// Contains an implementation of FloydТs cycle-finding algorithm.
		bool checkCircularParentDependency(ComponentID id) const;
#pragma endregion

	private:
		PersistentVector<ComponentID, constants::VERY_FREQUENT_COMPONENT> m_parent;
		PersistentVector<std::vector<ComponentID>, constants::VERY_FREQUENT_COMPONENT> m_children;

		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_localPosition;
		PersistentVector<Quaternion, constants::VERY_FREQUENT_COMPONENT> m_localRotation;
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_localScale;

		// примен€ть перемещение не сразу ко всем дет€м, а суммировать его, пока не потребуютс€ глобальные координаты - тогда-то все и посчитать
		//PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_pendingGlobalPositionChanges;

		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_globalPosition;
		PersistentVector<Quaternion, constants::VERY_FREQUENT_COMPONENT> m_globalRotation;
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_globalScale;

		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_forward;
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_right;
		PersistentVector<Vector3, constants::VERY_FREQUENT_COMPONENT> m_up;

		PersistentVector<Matrix4x4, constants::VERY_FREQUENT_COMPONENT> m_modelMatrix;
	};
} // namespace engine