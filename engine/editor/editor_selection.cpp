#include "editor_selection.h"

#include <engine/core/input/input_system.h>
#include <engine/editor/entity_selection.h>
#include <engine/editor/gui/editor_viewport_window.h>

namespace engine
{
	EditorSelectionManager::EditorSelectionManager(std::shared_ptr<EditorViewportsManager> editorViewports,
												   std::shared_ptr<InputSystem> inputSystem,
												   std::shared_ptr<EntitySelection> entitySelection)
		: m_editorViewports(std::move(editorViewports))
		, m_inputSystem(std::move(inputSystem))
		, m_entitySelection(std::move(entitySelection))
	{
		DEBUG_ASSERT(m_editorViewports != nullptr);
		DEBUG_ASSERT(m_inputSystem != nullptr);
		DEBUG_ASSERT(m_entitySelection != nullptr);
	}

	PixelInfo EditorSelectionManager::handleSelection()
	{
		const auto* window = m_editorViewports->hoveredViewport();
		if (window == nullptr)
			return PixelInfo();

		IFramebuffer* framebuffer = window->framebuffers().selectionFramebuffer.get();

		Vector2 mouse = window->screenToContent(m_inputSystem->mousePosition());
		return framebuffer->readPixel(mouse.x, mouse.y);
	}

	PixelInfo EditorSelectionManager::entityToColor(EntityID entity)
	{
		if (entity == EntityID())
			return PixelInfo();

		std::size_t value = entity.id() + 1; // 0 means no selection so have to add +1 here

		static constexpr std::uint16_t max = std::numeric_limits<std::uint16_t>::max();
		std::uint16_t r = max & (value >> 48);
		std::uint16_t g = max & (value >> 32);
		std::uint16_t b = max & (value >> 16);
		std::uint16_t a = max & (value);

		return { r, g, b, a };
	}

	EntityID EditorSelectionManager::colorToEntity(PixelInfo pixel)
	{
		std::uint16_t r = pixel.color[0];
		std::uint16_t g = pixel.color[1];
		std::uint16_t b = pixel.color[2];
		std::uint16_t a = pixel.color[3];
		std::size_t id = (static_cast<std::size_t>(r) << 48) +
						 (static_cast<std::size_t>(g) << 32) +
						 (static_cast<std::size_t>(b) << 16) +
						 (static_cast<std::size_t>(a));
		if (id == 0)
			return EntityID();
		return EntityID(id);
	}
} // namespace engine