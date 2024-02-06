#include "gizmo.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>

#include <engine/internal/helpers/assert.h>
#include <engine/internal/helpers/enum_helpers.h>

namespace engine
{
	Gizmo::Gizmo()
		: m_attachedTransform(ComponentID())
		, m_gizmoType(GizmoType::None)
		, m_attachedMatrix(nullptr)
	{}

	ComponentID Gizmo::attachedTo() const
	{
		return m_attachedTransform;
	}

	void Gizmo::attach(ComponentID to, Matrix4x4* newMatrix)
	{
		m_attachedTransform = to;
		m_attachedMatrix = newMatrix;
	}

	void Gizmo::detach()
	{
		m_attachedTransform = ComponentID();
		m_attachedMatrix = nullptr;
	}

	bool Gizmo::translate(Vector3 vec)
	{
		if (!checkAttachment())
			return false;

		// Axis availability
		std::uint8_t axis = static_cast<std::uint8_t>(GizmoType::Translate);

		if ((m_gizmoType & GizmoType::Translate) == GizmoType::None)
			return false;

		if ((m_gizmoType & GizmoType::TranslateX) == GizmoType::None)
		{
			// disable the X axis
			axis &= ~static_cast<std::uint8_t>(GizmoType::TranslateX);
			vec.x = 0;
		}
		if ((m_gizmoType & GizmoType::TranslateY) == GizmoType::None)
		{
			// disable the Y axis
			axis &= ~static_cast<std::uint8_t>(GizmoType::TranslateY);
			vec.y = 0;
		}
		if ((m_gizmoType & GizmoType::TranslateZ) == GizmoType::None)
		{
			// disable the Z axis
			axis &= ~static_cast<std::uint8_t>(GizmoType::TranslateZ);
			vec.z = 0;
		}
		
		if (axis == 0)
			return false;

		*m_attachedMatrix = glm::translate(*m_attachedMatrix, vec);

		return true;
	}

	bool Gizmo::checkAttachment()
	{
		if (m_attachedMatrix == nullptr ||
			m_attachedTransform == ComponentID())
		{
			ERROR_LOG("Cannot use the gizmo with no attached transform!");
			return false;
		}
		return true;
	}

	GizmoType Gizmo::gizmoType() const
	{
		return m_gizmoType;
	}

	void Gizmo::setGizmoType(GizmoType type)
	{
		m_gizmoType = type;
	}
} // namespace engine