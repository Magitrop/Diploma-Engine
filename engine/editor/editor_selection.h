#pragma once

#include <memory>

#include <engine/core/entity/entity.h>
#include <engine/editor/gizmo/gizmo.h>
#include <engine/internal/render/i_framebuffer.h>

namespace engine
{
	class EntityManager;
	class EntitySelection;
	class EditorViewportsManager;
	class InputSystemAccessor;
	class GizmoManager;
	class EditorSelectionManager final
	{
		// friends
	private:
		friend class Editor;

		// members
	private:
		explicit EditorSelectionManager(std::shared_ptr<EditorViewportsManager> editorViewports,
										std::shared_ptr<InputSystemAccessor> inputSystemAccessor,
										std::shared_ptr<EntitySelection> entitySelection,
										std::shared_ptr<GizmoManager> gizmoManager);

	public:
		void handleSelection();

		// Gets a selection color of the given entity.
		// Returns zero color if failed.
		static Vector4 entityToColor(EntityID entity);
		// Gets an Entity ID with given selection color.
		// Returns an invalid Entity ID if failed.
		static EntityID colorToEntity(PixelInfo pixel);

		// Gets an GizmoType with given selection color.
		// Returns the GizmoType::None if failed.
		static GizmoType colorToGizmo(std::uint8_t color);
		// Gets a selection color of the given gizmo type.
		// Returns zero color if failed.
		static Vector4 gizmoToColor(GizmoType type);

	private:
		bool isGizmoSelected() const;

	private:
		std::shared_ptr<EditorViewportsManager> m_editorViewports;
		std::shared_ptr<InputSystemAccessor> m_inputSystemAccessor;
		std::shared_ptr<EntitySelection> m_entitySelection;
		std::shared_ptr<GizmoManager> m_gizmoManager;

		PixelInfo m_lastSelectedPixelInfo;
	};
} // namespace engine