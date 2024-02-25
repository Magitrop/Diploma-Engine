#pragma once

#include <memory>

#include <engine/core/math/vector3.h>
#include <engine/core/math/matrix4x4.h>
#include <engine/core/components/component.h>

namespace engine
{
	enum class GizmoType : std::uint16_t
	{
		None = 0, // Don't show gizmo.

		TranslateX = 1 << 0, // Allow to drag an entity along the X axis.
		TranslateY = 1 << 1, // Allow to drag an entity along the Y axis.
		TranslateZ = 1 << 2, // Allow to drag an entity along the Z axis.
		Translate = TranslateX | // Allow to drag an entity.
					TranslateY |
					TranslateZ,

		RotateX = 1 << 3, // Allow to rotate an entity around the X axis.
		RotateY = 1 << 4, // Allow to rotate an entity around the Y axis.
		RotateZ = 1 << 5, // Allow to rotate an entity around the Z axis.
		Rotate = RotateX | // Allow to rotate an entity.
				 RotateY |
				 RotateZ,

		ScaleX = 1 << 6, // Allow to scale an entity along the X axis.
		ScaleY = 1 << 7, // Allow to scale an entity along the Y axis.
		ScaleZ = 1 << 8, // Allow to scale an entity along the Z axis.
		Scale = ScaleX | // Allow to scale an entity.
				ScaleY |
				ScaleZ,

		All = Translate | // Allow to translate, rotate and scale an entity.
			  Rotate |
			  Scale
	};

	class EntityManager;
	class Gizmo final
	{
		// friends
	private:
		friend class GizmoManager;

		// members
	private:
		explicit Gizmo();

	public:
		// Returns an ID of the attached transform component.
		ComponentID attachedTo() const;
		bool isAttached() const;

		GizmoType gizmoType() const;

	private:
		// Attaches this gizmo to the given entity transform.
		void attach(ComponentID to);
		// Detaches this gizmo from any transform.
		void detach();

		void setGizmoType(GizmoType type);

	private:
		ComponentID m_attachedTransform;
		GizmoType m_gizmoType;
	};
} // namespace engine