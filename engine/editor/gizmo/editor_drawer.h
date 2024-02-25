#pragma once

#include <memory>
#include <vector>

#include <engine/core/math/matrix4x4.h>
#include <engine/core/resources/mesh.h>
#include <engine/core/resources/material.h>

namespace engine
{
	class IFramebuffer;
	class IEditorDrawerContext
	{
	public:
		virtual ~IEditorDrawerContext() = 0;

		// Adds a box with given parameters to the drawer context.
		// The pivot of the box is its center point.
		virtual IEditorDrawerContext& box(Matrix4x4 transform,
										  MaterialID material) = 0;
		virtual IEditorDrawerContext& cone() = 0;
		virtual IEditorDrawerContext& line(Vertex start, Vertex end, MaterialID material) = 0;
		virtual IEditorDrawerContext& mesh(std::vector<Vertex> vertices,
										   std::vector<std::uint32_t> indices,
										   Matrix4x4 transform,
										   MaterialID material) = 0;
	};

	class IEditorDrawer
	{
	public:
		using Context = std::shared_ptr<IEditorDrawerContext>;

		[[nodiscard]] virtual Context& context() = 0;

		virtual void beginDraw() = 0;
		virtual void endDraw() = 0;

		virtual void drawContext(const Matrix4x4& projection, const Matrix4x4& view) = 0;
		// @param selectionFramebuffer: Is gizmo drawn to be present on the selection framebuffer?
		virtual void drawGizmo(const Matrix4x4& projection, const Matrix4x4& view, bool selectionFramebuffer) = 0;
		virtual void drawSpatialGrid(const Matrix4x4& projection, const Matrix4x4& view) = 0;
	};
} // namespace engine