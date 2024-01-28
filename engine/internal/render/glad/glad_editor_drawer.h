#pragma once

#include <engine/editor/gizmo/editor_drawer.h>

namespace engine
{
	class GladEditorDrawer final : public EditorDrawer
	{
	public:
		explicit GladEditorDrawer(std::shared_ptr<ResourceManager> resourceManager);

		virtual void drawSpatialGrid() override;

	private:
		std::shared_ptr<ResourceManager> m_resourceManager;
	};
} // namespace engine