#include "editor_selection.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/input/input_system_accessor.h>

#include <engine/editor/entity_selection.h>
#include <engine/editor/gizmo/gizmo_manager.h>
#include <engine/editor/gui/editor_viewport_window.h>

#include <engine/internal/core/constants/editor_constants.h>
#include <engine/internal/helpers/enum_helpers.h>

namespace engine
{
	EditorSelectionManager::EditorSelectionManager(std::shared_ptr<EditorViewportsManager> editorViewports,
												   std::shared_ptr<InputSystemAccessor> inputSystemAccessor,
												   std::shared_ptr<EntitySelection> entitySelection,
												   std::shared_ptr<GizmoManager> gizmoManager)
		: m_editorViewports(std::move(editorViewports))
		, m_inputSystemAccessor(std::move(inputSystemAccessor))
		, m_entitySelection(std::move(entitySelection))
		, m_gizmoManager(std::move(gizmoManager))
		, m_lastSelectedPixelInfo(PixelInfo())
	{
		DEBUG_ASSERT(m_editorViewports != nullptr);
		DEBUG_ASSERT(m_inputSystemAccessor != nullptr);
		DEBUG_ASSERT(m_entitySelection != nullptr);
		DEBUG_ASSERT(m_gizmoManager != nullptr);
	}

	void EditorSelectionManager::handleSelection()
	{
		const auto* window = m_editorViewports->hoveredViewport();
		if (window == nullptr)
			return;

		const auto& inputSystem = m_inputSystemAccessor->inputSystem();
		IFramebuffer* framebuffer = window->framebuffers().selectionFramebuffer.get();

		Vector2 mouse = window->screenToContent(inputSystem->mousePosition());
		PixelInfo selectedPixel = framebuffer->readPixel(mouse.x, mouse.y);

		if (!PixelInfo::compareColor(m_lastSelectedPixelInfo, selectedPixel))
		{
			m_lastSelectedPixelInfo = selectedPixel;
		}

		if (inputSystem->onMouseClick(MouseButton::Left) && !isGizmoSelected())
		{
			EntityID id = colorToEntity(m_lastSelectedPixelInfo);
			DEBUG_LOG("Selected: {}", id.id());
			DEBUG_LOG("{}", m_lastSelectedPixelInfo.color[3]);

			if ((inputSystem->modifiers() & KeyModifiers::Ctrl) == KeyModifiers::None)
			{
				m_entitySelection->deselectAll();
			}

			m_entitySelection->select(id);
		}
		else if (m_gizmoManager->isAttached() && inputSystem->onMouseButtonDown(MouseButton::Left))
		{
			GizmoType currentGizmo = colorToGizmo(m_lastSelectedPixelInfo.color[3]);
			m_gizmoManager->beginGizmoDragging(currentGizmo);
		}
		else if (m_gizmoManager->isGizmoDragging() && inputSystem->onMouseButtonUp(MouseButton::Left))
		{
			m_gizmoManager->endGizmoDragging();
		}
	}

	/* static */ Vector4 EditorSelectionManager::entityToColor(EntityID entity)
	{
		if (entity == EntityID())
			return PixelInfo();

		// 0 means no selection so have to add +1 here
		std::uint32_t value = entity.id() + 1;

		float r = ((value & 0x00FF0000) >> 16) * constants::SELECTION_CODE_STEP;
		float g = ((value & 0x0000FF00) >> 8 ) * constants::SELECTION_CODE_STEP;
		float b = ((value & 0x000000FF) >> 0 ) * constants::SELECTION_CODE_STEP;

		return { r, g, b, 0.0f };
	}

	/* static */ EntityID EditorSelectionManager::colorToEntity(PixelInfo pixel)
	{
		std::uint32_t id =
			pixel.color[0] * 256 * 256 +
			pixel.color[1] * 256 +
			pixel.color[2];

		// no selection
		if (id == 0)
			return EntityID();

		// +1 was added when encoded this entity so have to subtract -1 here
		return EntityID(id - 1);
	}

	/* static */ GizmoType EditorSelectionManager::colorToGizmo(std::uint8_t color)
	{
		switch (color)
		{
		case 1 << 0:				return GizmoType::TranslateX;
		case 1 << 1:				return GizmoType::TranslateY;
		case 1 << 2:				return GizmoType::TranslateZ;
		case (1 << 0) + (1 << 1):	return GizmoType::TranslateX | GizmoType::TranslateY;
		case (1 << 0) + (1 << 2):	return GizmoType::TranslateX | GizmoType::TranslateZ;
		case (1 << 1) + (1 << 2):	return GizmoType::TranslateY | GizmoType::TranslateZ;
		case (1 << 3) - 1:			return GizmoType::Translate;
		case (1 << 3) + 0:			return GizmoType::RotateX;
		case (1 << 3) + 1:			return GizmoType::RotateY;
		case (1 << 3) + 2:			return GizmoType::RotateZ;
		case 1 << 4:				return GizmoType::ScaleX;
		case 1 << 5:				return GizmoType::ScaleY;
		case 1 << 6:				return GizmoType::ScaleZ;
		default: return GizmoType::None;
		}
	}

	/* static */ Vector4 EditorSelectionManager::gizmoToColor(GizmoType type)
	{
		switch (type)
		{
		case GizmoType::TranslateX:							return constants::X_TRANSLATION_GIZMO_CODE;
		case GizmoType::TranslateY:							return constants::Y_TRANSLATION_GIZMO_CODE;
		case GizmoType::TranslateZ:							return constants::Z_TRANSLATION_GIZMO_CODE;
		case GizmoType::TranslateX | GizmoType::TranslateY: return constants::XY_TRANSLATION_GIZMO_CODE;
		case GizmoType::TranslateX | GizmoType::TranslateZ: return constants::XZ_TRANSLATION_GIZMO_CODE;
		case GizmoType::TranslateY | GizmoType::TranslateZ: return constants::YZ_TRANSLATION_GIZMO_CODE;
		case GizmoType::Translate:							return constants::XYZ_TRANSLATION_GIZMO_CODE;
		case GizmoType::RotateX:							return constants::X_ROTATION_GIZMO_CODE;
		case GizmoType::RotateY:							return constants::Y_ROTATION_GIZMO_CODE;
		case GizmoType::RotateZ:							return constants::Z_ROTATION_GIZMO_CODE;
		case GizmoType::ScaleX:								return constants::X_SCALING_GIZMO_CODE;
		case GizmoType::ScaleY:								return constants::Y_SCALING_GIZMO_CODE;
		case GizmoType::ScaleZ:								return constants::Z_SCALING_GIZMO_CODE;
		default:											return Vector4::zero();
		}
	}

	bool EditorSelectionManager::isGizmoSelected() const
	{
		return (m_lastSelectedPixelInfo.color[3] * 256) != 0;
	}
} // namespace engine