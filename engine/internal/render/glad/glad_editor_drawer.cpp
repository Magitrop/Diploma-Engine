#include "glad_editor_drawer.h"

#include <engine/core/resources/resource_manager.h>

#include <engine/editor/gizmo/gizmo_manager.h>

#include <engine/internal/helpers/assert.h>
#include <engine/internal/render/i_render_pipeline.h>

namespace engine
{
	GladEditorDrawerContext::GladEditorDrawerContext(PrimitiveAddedCallback onPrimitiveAdded)
		: m_onPrimitiveAdded(std::move(onPrimitiveAdded))
	{
		DEBUG_ASSERT(m_onPrimitiveAdded != nullptr);
	}

	GladEditorDrawerContext& GladEditorDrawerContext::box(Matrix4x4 transform,
														  MaterialID material)
	{
		static struct Box
		{
			std::vector<Vertex> vertices
			{
				// bottom face
				{ Vector3(0, 0, 0) },
				{ Vector3(1, 0, 0) },
				{ Vector3(1, 0, 1) },
				{ Vector3(0, 0, 1) },

				// top face
				{ Vector3(0, 1, 0) },
				{ Vector3(1, 1, 0) },
				{ Vector3(1, 1, 1) },
				{ Vector3(0, 1, 1) },
			};

			std::vector<std::uint32_t> indices
			{
				// bottom face
				0, 1, 2,
				1, 2, 3,

				// top face
				4, 5, 6,
				5, 6, 7,

				// front face
				0, 4, 7,
				4, 7, 3,

				// back face
				2, 6, 5,
				6, 5, 1,

				// left face
				1, 5, 4,
				5, 4, 0,

				// right face
				3, 7, 6,
				7, 6, 2
			};

			GLuint VAO;

			Box()
			{
				GLuint VBO, EBO;

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), &indices[0], GL_STATIC_DRAW);

				// positions
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
		} box;

		Primitive primitive = Primitive(
			box.VAO,
			box.indices,
			transform,
			material,
			DrawingType::Triangle
		);
		m_primitives.push_back(primitive);
		m_onPrimitiveAdded();

		return *this;
	}

	GladEditorDrawerContext& GladEditorDrawerContext::cone()
	{
		return *this;
	}

	GladEditorDrawerContext& GladEditorDrawerContext::line()
	{
		return *this;
	}

	GladEditorDrawerContext& GladEditorDrawerContext::mesh(std::vector<Vertex> vertices,
														   std::vector<std::uint32_t> indices,
														   Matrix4x4 transform,
														   MaterialID material)
	{
		struct Mesh
		{
			GLuint VAO;
			Mesh(std::vector<Vertex> vertices, std::vector<std::uint32_t> indices)
			{
				GLuint VBO, EBO;

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), &indices[0], GL_STATIC_DRAW);

				// positions
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
		} mesh(std::move(vertices), indices);

		Primitive primitive = Primitive(
			mesh.VAO,
			std::move(indices),
			transform,
			material,
			DrawingType::Triangle
		);
		m_primitives.push_back(primitive);
		m_onPrimitiveAdded();

		return *this;
	}

	GladEditorDrawer::GladEditorDrawer(std::shared_ptr<IRenderPipeline> renderPipeline,
									   std::shared_ptr<ResourceManager> resourceManager,
									   std::shared_ptr<GizmoManager> gizmoManager)
		: m_resourceManager(std::move(resourceManager))
		, m_renderPipeline(std::move(renderPipeline))
		, m_gizmoManager(std::move(gizmoManager))
	{
		DEBUG_ASSERT(m_resourceManager != nullptr);
		DEBUG_ASSERT(m_renderPipeline != nullptr);
		DEBUG_ASSERT(m_gizmoManager != nullptr);

		m_context = std::shared_ptr<GladEditorDrawerContext>(
			new GladEditorDrawerContext(
				[this]()
				{
					sortPrimitivesByRenderQueue();
				})
		);

		drawSpatialGrid();
	}

	void GladEditorDrawer::drawSpatialGrid()
	{
		std::vector<Vertex> vertices
		{
			{ Vector3(0, 0, 0) },
			{ Vector3(0, 0, 0) },
			{ Vector3(0, 0, 0) },
			{ Vector3(0, 0, 0) },
			{ Vector3(0, 0, 0) },
			{ Vector3(0, 0, 0) }
		};

		std::vector<std::uint32_t> indices
		{
			0, 1, 2,
			3, 4, 5
		};

		m_context->mesh(
			std::move(vertices),
			std::move(indices),
			Matrix4x4(1),
			m_resourceManager->findMaterial("editor_grid")
		);
	}

	void GladEditorDrawer::drawGizmo()
	{

	}

	void GladEditorDrawer::sortPrimitivesByRenderQueue()
	{
		using Primitive = GladEditorDrawerContext::Primitive;
		GladEditorDrawerContext* context = static_cast<GladEditorDrawerContext*>(m_context.get());
		std::sort(
			context->m_primitives.begin(),
			context->m_primitives.end(),
			[this](const Primitive& left, const Primitive& right) -> bool
			{
				auto a = m_resourceManager->getMaterialByID(left.material)->renderQueue();
				auto b = m_resourceManager->getMaterialByID(right.material)->renderQueue();
				return a < b;
			});
	}

	IEditorDrawer::Context& GladEditorDrawer::context()
	{
		return m_context;
	}
} // namespace engine