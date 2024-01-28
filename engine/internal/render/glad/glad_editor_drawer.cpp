#include "glad_editor_drawer.h"

#include <engine/internal/helpers/assert.h>

namespace engine
{
	GladEditorDrawer::GladEditorDrawer(std::shared_ptr<ResourceManager> resourceManager)
		: m_resourceManager(std::move(resourceManager))
	{
		DEBUG_ASSERT(m_resourceManager != nullptr);
	}

	void GladEditorDrawer::drawSpatialGrid()
	{

	}
} // namespace engine