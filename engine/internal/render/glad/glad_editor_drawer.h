#pragma once

#include <engine/editor/gizmo/editor_drawer.h>
#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/internal/render/glad/glad_editor_drawer_primitives.h>
#include <engine/internal/render/glad/glad_material.h>

#include <engine/dependencies/glad/include/glad/glad.h>

namespace engine
{
	class GladEditorDrawerContext final : public IEditorDrawerContext
	{
		// friends
	private:
		friend class GladEditorDrawer;
		 
		// members
	public:
		using PrimitiveAddedCallback = std::function<void()>;

		enum class DrawingType : std::uint8_t
		{
			None = 0,
			Points = 1, // GL_POINTS
			Line = 2, // GL_LINE_STRIP
			Triangle = 3, // GL_TRIANGLES
			TriangleStrip = 4, // GL_TRIANGLE_STRIP
		};

		struct Primitive
		{
			GLuint VAO;
			std::vector<std::uint32_t> indices;
			Matrix4x4 transform;
			MaterialID material;
			DrawingType drawingType;
			// TODO: line width
		};

		explicit GladEditorDrawerContext(PrimitiveAddedCallback onPrimitiveAdded);

		virtual GladEditorDrawerContext& box(Matrix4x4 transform,
											 MaterialID material) override;
		virtual GladEditorDrawerContext& cone() override;
		virtual GladEditorDrawerContext& line() override;
		virtual GladEditorDrawerContext& mesh(std::vector<Vertex> vertices,
											  std::vector<std::uint32_t> indices,
											  Matrix4x4 transform,
											  MaterialID material) override;

		const std::vector<Primitive>& primitives() const { return m_primitives; }

	private:
		std::vector<Primitive> m_primitives;
		PrimitiveAddedCallback m_onPrimitiveAdded;
	};

	class GizmoManager;
	class IRenderPipeline;
	class ResourceManager;
	class GladEditorDrawer final : public IEditorDrawer
	{
	public:
		explicit GladEditorDrawer(std::shared_ptr<IRenderPipeline> renderPipeline,
								  std::shared_ptr<ResourceManager> resourceManager,
								  std::shared_ptr<GizmoManager> gizmoManager);

		[[nodiscard]] virtual Context& context() override;

	private:
		virtual void drawSpatialGrid() override;
		virtual void drawGizmo() override;

		void sortPrimitivesByRenderQueue();
		
	private:
		std::shared_ptr<IRenderPipeline> m_renderPipeline;
		std::shared_ptr<ResourceManager> m_resourceManager;
		std::shared_ptr<GizmoManager> m_gizmoManager;
		Context m_context;
	};
} // namespace engine