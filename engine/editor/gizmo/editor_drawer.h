#pragma once

#include <memory>
#include <vector>

namespace engine
{
	struct EditorDrawerPrimitive
	{
		virtual ~EditorDrawerPrimitive() = 0;
	};

	struct EditorDrawerContext final
	{
		std::vector<EditorDrawerPrimitive> m_primitives;
	};

	class IRenderPipeline;
	class ResourceManager;
	class EditorDrawer
	{
	public:
		virtual void drawSpatialGrid() = 0;

		[[nodiscard]] EditorDrawerContext obtainContext();

	protected:
		EditorDrawerContext m_drawerContext{};
	};
} // namespace engine