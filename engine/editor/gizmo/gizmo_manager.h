#pragma once

#include <memory>

#include <engine/core/components/component.h>

namespace engine
{
	class EntityManager;
	class Gizmo;
	class GizmoManager final
	{
		// friends
	private:
		friend class Editor;

		// members
	public:
		explicit GizmoManager(std::shared_ptr<EntityManager> entityManager);

		const Gizmo* gizmo() const;

	private:
		void attach(ComponentID to);

	private:
		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<Gizmo> m_gizmo;
	};
} // namespace engine
