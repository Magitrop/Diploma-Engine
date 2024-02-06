#include "gizmo_manager.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>

#include <engine/editor/gizmo/gizmo.h>

#include <engine/internal/helpers/assert.h>

namespace engine
{
	GizmoManager::GizmoManager(std::shared_ptr<EntityManager> entityManager)
		: m_entityManager(std::move(entityManager))
	{
		DEBUG_ASSERT(m_entityManager != nullptr);

		m_gizmo = std::shared_ptr<Gizmo>(new Gizmo());
	}

	const Gizmo* GizmoManager::gizmo() const
	{
		return m_gizmo.get();
	}

	void GizmoManager::attach(ComponentID to)
	{
		Matrix4x4* matrix = &m_entityManager->getComponentManager<Transform>()->modelMatrix(to);
		m_gizmo->attach(to, matrix);
	}
} // namespace engine