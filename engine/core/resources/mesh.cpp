#include "mesh.h"

namespace engine
{
	MeshID::MeshID(std::size_t index)
		: m_id(index)
	{}

	MeshID::MeshID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	std::size_t MeshID::id() const
	{
		return m_id;
	}

	MeshID::operator std::size_t() const
	{
		return m_id;
	}

	bool MeshID::isValid() const
	{
		return m_id != static_cast<std::size_t>(-1);
	}

	Mesh::Mesh()
	{
	}

	Mesh::Mesh(std::vector<Vertex> vertices)
		: m_vertices(std::move(vertices))
	{
	}
} // namespace engine