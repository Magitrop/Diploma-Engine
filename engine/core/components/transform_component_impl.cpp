#include "transform_component_impl.h"

namespace engine
{
	namespace
	{
		template<typename T, typename U>
		using ConstVariant = std::conditional_t<std::is_const_v<T>, const U, U>;

		template<typename Vector>
		ConstVariant<Vector, typename Vector::ValueType>* getDataPtr(Vector& vector, std::size_t index)
		{
			const auto& it = vector.at(index);
			if (it == vector.end() || it->empty())
			{
				return nullptr;
			}
			return &vector.at(index)->get();
		}
	}

	void TransformInternal::attachComponent(EntityID entity, ComponentID component) /* override */
	{
		m_modelMatrix.push(Matrix4x4(1.0f));

		m_localPosition.push(Vector3::zero());
		m_localRotation.push(Quaternion({ 0, 0, 0 }));
		m_localScale.push(Vector3::one());

		m_globalPosition.push(Vector3::zero());
		m_globalRotation.push(Quaternion({ 0, 0, 0 }));
		m_globalScale.push(Vector3::one());

		m_forward.push(Vector3::forward());
		m_right.push(Vector3::right());
		m_up.push(Vector3::up());

		m_parent.push(ComponentID());
		m_children.push({});
	}

	void TransformInternal::detachComponent(EntityID entity, ComponentID component) /* override */
	{
		m_modelMatrix.remove(m_modelMatrix.begin() + component.id());

		m_localPosition.remove(m_localPosition.begin() + component.id());
		m_localRotation.remove(m_localRotation.begin() + component.id());
		m_localScale.remove(m_localScale.begin() + component.id());

		m_globalPosition.remove(m_globalPosition.begin() + component.id());
		m_globalRotation.remove(m_globalRotation.begin() + component.id());
		m_globalScale.remove(m_globalScale.begin() + component.id());

		m_forward.remove(m_forward.begin() + component.id());
		m_right.remove(m_right.begin() + component.id());
		m_up.remove(m_up.begin() + component.id());

		m_parent.remove(m_parent.begin() + component.id());
		m_children.remove(m_children.begin() + component.id());
	}

	Matrix4x4 TransformInternal::modelMatrix(ComponentID id) const
	{
		if (const Matrix4x4* model = getDataPtr(m_modelMatrix, id))
			return *model;
		ERROR_LOG("Cannot get the model matrix: the {} transform ID is invalid!", id.id());
		return Matrix4x4(0.0f);
	}

	Vector3 TransformInternal::position(ComponentID id) const
	{
		if (const Vector3* position = getDataPtr(m_globalPosition, id))
			return *position;
		ERROR_LOG("Cannot get the global position: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Vector3 TransformInternal::localPosition(ComponentID id) const
	{
		if (const Vector3* position = getDataPtr(m_localPosition, id))
			return *position;
		ERROR_LOG("Cannot get the local position: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Quaternion TransformInternal::rotation(ComponentID id) const
	{
		if (const Quaternion* rotation = getDataPtr(m_globalRotation, id))
			return *rotation;
		ERROR_LOG("Cannot get the global rotation: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Quaternion TransformInternal::localRotation(ComponentID id) const
	{
		if (const Quaternion* rotation = getDataPtr(m_localRotation, id))
			return *rotation;
		ERROR_LOG("Cannot get the local rotation: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Vector3 TransformInternal::scale(ComponentID id) const
	{
		if (const Vector3* scale = getDataPtr(m_globalScale, id))
			return *scale;
		ERROR_LOG("Cannot get the global scale: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Vector3 TransformInternal::localScale(ComponentID id) const
	{
		if (const Vector3* scale = getDataPtr(m_localScale, id))
			return *scale;
		ERROR_LOG("Cannot get the local scale: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Vector3 TransformInternal::forward(ComponentID id) const
	{
		if (const Vector3* forward = getDataPtr(m_forward, id))
			return *forward;
		ERROR_LOG("Cannot get the forward direction vector: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Vector3 TransformInternal::up(ComponentID id) const
	{
		if (const Vector3* up = getDataPtr(m_up, id))
			return *up;
		ERROR_LOG("Cannot get the up direction vector: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	Vector3 TransformInternal::right(ComponentID id) const
	{
		if (const Vector3* right = getDataPtr(m_right, id))
			return *right;
		ERROR_LOG("Cannot get the right direction vector: the {} transform ID is invalid!", id.id());
		return Vector3::invalid();
	}

	ComponentID TransformInternal::parent(ComponentID id) const
	{
		auto* res = getDataPtr(m_parent, id);
		return res ? *res : ComponentID();
	}

	void TransformInternal::setParent(ComponentID child, ComponentID parent)
	{
		if (auto* parentPtr = getDataPtr(m_parent, child))
		{
			ComponentID prevParent = *parentPtr;
			*parentPtr = parent;
			if (checkCircularParentDependency(child))
			{
				*parentPtr = prevParent;
				ERROR_LOG("Cannot set parent: there is a circular parent dependency between the {} and {} transform!", child.id(), parent.id());
			}
			else
			{
				auto prevIt = m_children.at(prevParent);
				if (prevIt != m_children.end() && !prevIt->empty())
				{
					auto& vec = prevIt->get();
					vec.erase(std::find(vec.begin(), vec.end(), child));
				}

				auto newIt = m_children.at(parent);
				if (newIt != m_children.end() && !newIt->empty())
				{
					newIt->get().push_back(child);
				}
			}
		}
	}

	void TransformInternal::translateLocalBy(ComponentID id, Vector3 value)
	{
		if (auto* localPositionPtr = getDataPtr(m_localPosition, id))
		{
			*localPositionPtr += value;
			updateChildren(id);
		}
	}

	void TransformInternal::rotateBy(ComponentID id, Quaternion value)
	{
		if (auto* localRotationPtr = getDataPtr(m_localRotation, id))
		{
			*localRotationPtr *= value;
			updateChildren(id);
		}
	}

	void TransformInternal::scaleBy(ComponentID id, Vector3 value)
	{

	}

	void TransformInternal::updateRelativeDirections(ComponentID id)
	{
		Vector3& forward = m_forward.at(id)->get();
		Vector3& right = m_right.at(id)->get();
		Vector3& up = m_up.at(id)->get();

		forward = Vector3::forward();
		ComponentID parentID = id;
		do
		{
			if (const Quaternion* rotation = getDataPtr(m_localRotation, parentID))
			{
				forward = (*rotation) * forward;
			}
			else
			{
				ERROR_LOG("Cannot update direction vectors: the {} transform ID is invalid!", id.id());
				return;
			}
			parentID = parent(parentID);
		} while (parentID.isValid());

		right = glm::cross(forward, Vector3::up());
		up = glm::cross(forward, right);

		forward.normalize();
		right.normalize();
		up.normalize();
	}

	void TransformInternal::updateModelMatrix(ComponentID id)
	{
		Matrix4x4& matrix = m_modelMatrix.at(id)->get();

		const auto& scale = this->scale(id);

		Matrix4x4 newMatrix(1.0f);
		newMatrix = glm::scale(newMatrix, scale);
		
		auto rotate =
			[](Matrix4x4& matrix, Quaternion rotation)
			{
				Vector3 euler = glm::eulerAngles(rotation);
				matrix = glm::rotate(matrix, euler.z, Vector3::forward());
				matrix = glm::rotate(matrix, euler.y, Vector3::up());
				matrix = glm::rotate(matrix, euler.x, Vector3::right());
			};

		if (ComponentID parent = this->parent(id);
			parent.isValid())
		{
			const auto& position = this->localPosition(id);
			const auto& rotation = this->localRotation(id);
			const auto& parentPosition = this->position(parent);
			const auto& parentRotation = this->rotation(parent);
			const auto& parentScale = this->scale(parent);
			newMatrix = glm::translate(newMatrix, parentPosition / parentScale);
			rotate(newMatrix, parentRotation);
			newMatrix = glm::translate(newMatrix, position / scale);
			rotate(newMatrix, rotation);
		}
		else
		{
			const auto& position = this->position(id);
			const auto& rotation = this->rotation(id);
			newMatrix = glm::translate(newMatrix, position / scale);
			rotate(newMatrix, rotation);
		}

		matrix = newMatrix;
	}

	void TransformInternal::updateChildren(ComponentID id)
	{
		auto update =
			[this](ComponentID child, ComponentID parent)
			{
				updatePosition(child, parent);
				updateRotation(child, parent);
				updateScale(child, parent);

				updateModelMatrix(child);
				updateRelativeDirections(child);
			};

		update(id, parent(id));

		if (std::vector<ComponentID>* children = getDataPtr(m_children, id))
		{
			for (ComponentID child : *children)
			{
				update(child, id);
				updateChildren(child);
			}
		}
	}

	void TransformInternal::updatePosition(ComponentID child, ComponentID parent)
	{
		DEBUG_ASSERT(child.isValid());

		Vector3& globalChild = m_globalPosition.at(child)->get();
		Vector3& localChild = m_localPosition.at(child)->get();
		if (parent.isValid())
		{
			Vector3& globalParent = m_globalPosition.at(parent)->get();
			globalChild = globalParent + localChild;
		}
		else // no parent
		{
			globalChild = localChild;
		}
	}

	void TransformInternal::updateRotation(ComponentID child, ComponentID parent)
	{
		Quaternion& globalChild = m_globalRotation.at(child)->get();
		Quaternion& localChild = m_localRotation.at(child)->get();
		if (parent.isValid())
		{
			Quaternion& globalParent = m_globalRotation.at(parent)->get();
			globalChild = globalParent * localChild;
		}
		else // no parent
		{
			globalChild = localChild;
		}
	}

	void TransformInternal::updateScale(ComponentID child, ComponentID parent)
	{
		Vector3& globalChild = m_globalScale.at(child)->get();
		Vector3& localChild = m_localScale.at(child)->get();
		if (parent.isValid())
		{
			Vector3& globalParent = m_globalScale.at(parent)->get();
			globalChild = globalParent * localChild;
		}
		else // no parent
		{
			globalChild = localChild;
		}
	}

	bool TransformInternal::checkCircularParentDependency(ComponentID id) const
	{
		ComponentID slow = id;
		ComponentID fast = id;

		while (slow != ComponentID() &&
			   fast != ComponentID() &&
			   parent(fast) != ComponentID())
		{
			slow = parent(slow);
			fast = parent(parent(fast));
			if (slow == fast)
				return true;
		}

		return false;
	}
} // namespace engine