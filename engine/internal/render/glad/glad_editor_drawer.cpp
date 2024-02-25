#include "glad_editor_drawer.h"

#include <engine/core/resources/resource_manager.h>

#include <engine/editor/gizmo/gizmo_manager.h>
#include <engine/editor/gizmo/gizmo_elements.h>

#include <engine/internal/helpers/assert.h>
#include <engine/internal/helpers/enum_helpers.h>

#include <engine/internal/render/i_render_pipeline.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

#include <engine/internal/core/constants/editor_constants.h>
#include <engine/dependencies/glm/glm/ext/matrix_transform.hpp>

namespace engine
{
	/* static */ GLenum GladEditorDrawerContext::drawingType(DrawingType type)
	{
		switch (type)
		{
		case DrawingType::Points:
			return GL_POINTS;
		case DrawingType::Line:
			return GL_LINE_STRIP;
		case DrawingType::Triangle:
			return GL_TRIANGLES;
		case DrawingType::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		default:
			ERROR_LOG("Incorrect drawing type!");
			return GL_POINTS;
		}
	}

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
				0, 3, 2,
				2, 1, 0,

				// top face
				4, 5, 6,
				6, 7, 4,

				// left face
				3, 0, 4,
				4, 7, 3,

				// right face
				2, 6, 5,
				5, 1, 2,

				// front face
				0, 1, 5,
				5, 4, 0,

				// back face
				2, 3, 7,
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

	GladEditorDrawerContext& GladEditorDrawerContext::line(Vertex start, Vertex end, MaterialID material)
	{
		struct Line
		{
			std::array<Vertex, 2> vertices;
			std::vector<std::uint32_t> indices { 0, 1 };

			GLuint VAO;
			Line(Vertex a, Vertex b)
			{
				vertices[0] = a;
				vertices[1] = b;

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
		} line(start, end);

		Primitive primitive = Primitive(
			line.VAO,
			line.indices,
			Matrix4x4(1.0f),
			material,
			DrawingType::Line
		);
		m_primitives.push_back(primitive);
		m_onPrimitiveAdded();

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
	}

	void GladEditorDrawer::drawSpatialGrid(const Matrix4x4& projection, const Matrix4x4& view)
	{
		constexpr std::array<std::uint32_t, 6> indices =
		{
			0, 1, 2,
			3, 4, 5
		};

		static GLuint VAO = 0;
		if (!glIsVertexArray(VAO))
			glGenVertexArrays(1, &VAO);

		static GladResourceManager* resourceManager = static_cast<GladResourceManager*>(m_resourceManager.get());
		static const GladMaterialImpl* material = resourceManager->getMaterial("editor_grid");
		static const GladShader* shader = material->shader();

		material->useShader();

		shader->setMatrix4x4("projection", projection);
		shader->setMatrix4x4("view", view);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
	}

	void GladEditorDrawer::beginDraw()
	{

	}

	void GladEditorDrawer::endDraw()
	{
		auto context = static_cast<GladEditorDrawerContext*>(m_context.get());
		DEBUG_ASSERT(context != nullptr);

		for (auto& primitive : context->m_primitives)
			glDeleteVertexArrays(1, &primitive.VAO);
		context->m_primitives.clear();
	}

	void GladEditorDrawer::drawContext(const Matrix4x4& projection, const Matrix4x4& view)
	{
		auto context = static_cast<GladEditorDrawerContext*>(m_context.get());
		DEBUG_ASSERT(context != nullptr);

		auto resourceManager = static_cast<GladResourceManager*>(m_resourceManager.get());
		DEBUG_ASSERT(context != nullptr);

		std::int8_t renderQueueIndex = 0;
		for (auto& primitive : context->m_primitives)
		{
			const GladMaterialImpl* material = resourceManager->getMaterial(primitive.material);
			const GladShader* shader = material->shader();

			if (material->renderQueue() != renderQueueIndex)
			{
				renderQueueIndex = material->renderQueue();
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			const GLuint VAO = primitive.VAO;
			const auto* indices = &primitive.indices;

			material->useShader();

			shader->setMatrix4x4("projection", projection);
			shader->setMatrix4x4("view", view);
			shader->setMatrix4x4("model", primitive.transform);

			glBindVertexArray(VAO);
			glDrawElements(GladEditorDrawerContext::drawingType(primitive.drawingType), indices->size(), GL_UNSIGNED_INT, &(*indices)[0]);
		}
	}

	void GladEditorDrawer::drawGizmo(const Matrix4x4& projection, const Matrix4x4& view, bool selectionFramebuffer)
	{
		if (!m_gizmoManager->isAttached())
			return;

		static GladResourceManager* resourceManager = static_cast<GladResourceManager*>(m_resourceManager.get());
		static const GladMaterialImpl* material = resourceManager->getMaterial("gizmo");
		static const GladShader* shader = material->shader();

		material->useShader();

		Matrix4x4 model = m_gizmoManager->getAttachedMatrix();

		Vector4 gizmoPosition = model[3];
		Vector4 cameraPosition = glm::inverse(view) * Vector4(0, 0, 0, 1);
		float distance = Vector3(cameraPosition - gizmoPosition).magnitude();
		model = glm::scale(model, Vector3(distance * 0.2f));

		shader->setMatrix4x4("projection", projection);
		shader->setMatrix4x4("view", view);
		shader->setMatrix4x4("model", model);

		struct Buffers
		{
			GLuint VAO = 0;
			GLuint VBO = 0;
			GLuint EBO = 0;
			std::vector<Vertex> vertices;
			std::vector<std::uint32_t> indices;

			Buffers(std::vector<Vertex> vertices, std::vector<std::uint32_t> indices)
			{
				this->vertices = std::move(vertices);
				this->indices = std::move(indices);

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(std::uint32_t), &this->indices[0], GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

			~Buffers()
			{
				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
				glDeleteBuffers(1, &EBO);
			}
		};

		auto draw = [selectionFramebuffer]<typename Element>()
		{
			static Buffers buffers(Element::vertices(), Element::indices());
			Vector4 color =
				selectionFramebuffer
				? Element::selectionColor
				: Element::drawingColor;

			shader->setVector4("axis", color);
			glBindVertexArray(buffers.VAO);
			glDrawElements(GL_LINES, buffers.indices.size(), GL_UNSIGNED_INT, &buffers.indices[0]);
		};

		GizmoType type = m_gizmoManager->gizmo()->gizmoType();
		if ((type | GizmoType::TranslateX) != GizmoType::None)
		{
			using Element = GizmoElement<GizmoType::TranslateX>;
			draw.template operator()<Element>();
		}
		if ((type | GizmoType::TranslateY) != GizmoType::None)
		{
			using Element = GizmoElement<GizmoType::TranslateY>;
			draw.template operator()<Element>();
		}
		if ((type | GizmoType::TranslateZ) != GizmoType::None)
		{
			using Element = GizmoElement<GizmoType::TranslateZ>;
			draw.template operator()<Element>();
		}

		glBindVertexArray(0);
		glUseProgram(0);
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