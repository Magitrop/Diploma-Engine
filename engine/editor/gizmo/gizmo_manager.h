#pragma once

#include <memory>

#include <engine/core/components/component.h>
#include <engine/editor/gizmo/gizmo.h>
#include <engine/internal/helpers/dependencies.h>
#include <engine/internal/helpers/signal.h>

namespace engine
{
	class Editor;
	class EntityManager;
	class InputSystem;
	class Transform;
	class GizmoManager final : public Dependencies<EntityManager, InputSystem, Editor>
	{
		// friends
	private:
		friend class Editor;
		friend class EditorSelectionManager;
		friend class EntitySelection;

		// members
	public:
		explicit GizmoManager(DependencyContext dependencies);

		void tick();

		const Gizmo* gizmo() const;
		Matrix4x4 getAttachedMatrix() const;
		bool isAttached() const;
		bool isGizmoDragging() const;

		void setGizmoType(GizmoType type);

	private:
		void beginGizmoDragging(GizmoType type);
		void endGizmoDragging();

		void attach(ComponentID to);
		void detach();

	private:
		std::unique_ptr<Gizmo> m_gizmo;

		Vector3 m_lastWorldPoint;
		Vector3 m_planeNormal;

		SignalEmitter<void(GizmoType)> m_onBeginGizmoDragging;
		SignalEmitter<void()> m_onEndGizmoDragging;

		GizmoType m_gizmoDraggingMode = GizmoType::None;
	};
} // namespace engine
