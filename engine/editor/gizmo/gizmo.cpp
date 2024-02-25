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
	{}

	ComponentID Gizmo::attachedTo() const
	{
		return m_attachedTransform;
	}

	bool Gizmo::isAttached() const
	{
		return m_attachedTransform != ComponentID();
	}

	void Gizmo::attach(ComponentID to)
	{
		m_attachedTransform = to;
	}

	void Gizmo::detach()
	{
		m_attachedTransform = ComponentID();
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