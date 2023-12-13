#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <engine/core/math/vector3.h>

namespace engine
{
	struct Vertex
	{
		Vector3 position;
	};

	// A lightweight wrapper for an actual mesh in the Resource Manager.
	class MeshID final
	{
	public:
		explicit MeshID(std::size_t index);

		// Creates an invalid MeshID.
		explicit MeshID();

		std::size_t id() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};

	class Mesh final
	{
	public:
		// Creates an empty Mesh.
		explicit Mesh();

		explicit Mesh(std::vector<Vertex> vertices);

	private:
		std::vector<Vertex> m_vertices;
	};
} // namespace engine
