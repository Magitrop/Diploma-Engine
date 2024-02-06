#pragma once

#include <memory>

#include <engine/core/entity/entity.h>
#include <engine/internal/render/i_framebuffer.h>

namespace engine
{
	class EditorViewportsManager;
	class EntitySelection;
	class InputSystem;
	class EditorSelectionManager final
	{
		// friends
	private:
		friend class Editor;

		// members
	private:
		explicit EditorSelectionManager(std::shared_ptr<EditorViewportsManager> editorViewports,
										std::shared_ptr<InputSystem> inputSystem,
										std::shared_ptr<EntitySelection> entitySelection);

	public:
		PixelInfo handleSelection();

	private:
		// Gets a selection color of the given entity.
		// Returns zero color if failed.
		PixelInfo entityToColor(EntityID entity);
		// Gets an Entity ID with given selection color.
		// Returns an invalid Entity ID if failed.
		EntityID colorToEntity(PixelInfo pixel);

		std::shared_ptr<EditorViewportsManager> m_editorViewports;
		std::shared_ptr<InputSystem> m_inputSystem;
		std::shared_ptr<EntitySelection> m_entitySelection;
	};
} // namespace engine