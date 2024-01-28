#include "editor_drawer.h"

#include <engine/core/resources/resource_manager.h>
#include <engine/internal/helpers/assert.h>

namespace engine
{
	EditorDrawerPrimitive::~EditorDrawerPrimitive()
	{}

	EditorDrawerContext EditorDrawer::obtainContext()
	{
		EditorDrawerContext context(std::move(m_drawerContext));
		m_drawerContext = EditorDrawerContext();
		return context;
	}
} // namespace engine