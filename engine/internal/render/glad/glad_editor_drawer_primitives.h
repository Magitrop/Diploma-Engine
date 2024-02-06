#pragma once

#include <engine/editor/gizmo/editor_drawer_primitives.h>
#include <engine/dependencies/glad/include/glad/glad.h>

namespace engine
{
	struct GladEditorDrawerPrimitive : public EditorDrawerPrimitive
	{
		GLuint VAO;
	};
} // namespace engine