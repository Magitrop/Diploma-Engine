#include "glad_mesh.h"

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/assert.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component.h>

namespace engine
{
	GladMeshAccessor::GladMeshAccessor(GladMeshImpl* impl)
	{
		DEBUG_ASSERT(impl != nullptr);

		m_impl = impl;
	}

	void GladMeshAccessor::setVertices(std::vector<Vertex>&& vertices, std::vector<std::uint32_t>&& indices)
	{
		m_impl->setVertices(std::move(vertices), std::move(indices));
	}

	GladMeshImpl::GladMeshImpl()
		: m_VAO(GLuint())
		, m_VBO(GLuint())
		, m_EBO(GLuint())
		, m_id(MeshID())
	{}

	GladMeshImpl::GladMeshImpl(MeshID id)
		: m_VAO(GLuint())
		, m_VBO(GLuint())
		, m_EBO(GLuint())
		, m_id(id)
	{}

	GladMeshImpl::~GladMeshImpl()
	{
		if (glIsBuffer(m_VBO))
			glDeleteBuffers(1, &m_VBO);

		if (glIsVertexArray(m_VAO))
			glDeleteVertexArrays(1, &m_VAO);

		if (glIsBuffer(m_EBO))
			glDeleteBuffers(1, &m_EBO);
	}

	void GladMeshImpl::setVertices(std::vector<Vertex>&& vertices, std::vector<std::uint32_t>&& indices)
	{
		m_vertices = std::move(vertices);
		m_indices = std::move(indices);

		if (glIsBuffer(m_VBO))
			glDeleteBuffers(1, &m_VBO);
		if (glIsVertexArray(m_VAO))
			glDeleteVertexArrays(1, &m_VAO);
		if (glIsBuffer(m_EBO))
			glDeleteBuffers(1, &m_EBO);

		glGenBuffers(1, &m_VBO);
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(std::uint32_t), &m_indices[0], GL_STATIC_DRAW);

		// positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_onMeshChanged(m_id);
	}

	GLuint GladMeshImpl::VAO() const
	{
		return m_VAO;
	}

	GLuint GladMeshImpl::VBO() const
	{
		return m_VBO;
	}

	GLuint GladMeshImpl::EBO() const
	{
		return m_EBO;
	}

	const std::vector<std::uint32_t>* GladMeshImpl::indices() const
	{
		return &m_indices;
	}

	void GladMeshImpl::connectOnMeshChanged(std::function<void(MeshID)> action) const
	{
		m_onMeshChanged += action;
	}
} // namespace engine
