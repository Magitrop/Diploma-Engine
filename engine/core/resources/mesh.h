#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

#include <engine/core/math/vector3.h>
#include <engine/internal/helpers/non_copyable.h>

namespace engine
{
	struct Vertex
	{
		Vector3 position;
		//Vector3 normal;
	};

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

	// A lightweight wrapper for an actual mesh in the Resource Manager.
	class MeshAccessor
	{
	public:
		virtual void setVertices(std::vector<Vertex>&& vertices) {};
	};
} // namespace engine
