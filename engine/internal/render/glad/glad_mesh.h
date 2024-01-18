#pragma once

#include <unordered_map>

#include <engine/core/resources/resource_manager.h>
#include <engine/internal/helpers/signal.h>
#include <engine/internal/render/glad/glad_graphic_api.h>

namespace engine
{
	class GladMeshImpl;
	class GladMeshRenderer;
	class GladMeshAccessor final : public MeshAccessor
	{
		// friends
	private:
		friend class GladRenderPipeline;
		friend class GladResourceManager;

		// members
	public:
		GladMeshAccessor(GladMeshImpl* impl);

		virtual void setVertices(std::vector<Vertex>&& vertices, std::vector<std::uint32_t>&& indices) override;

	private:
		GladMeshImpl* m_impl;
	};

	class GladMeshImpl final
	{
		// friends
	private:
		friend class GladRenderPipeline;
		friend class GladResourceManager;

		// members
	public:
		GladMeshImpl();
		GladMeshImpl(MeshID id);
		~GladMeshImpl();

		void setVertices(std::vector<Vertex>&& vertices, std::vector<std::uint32_t>&& indices);

		GLuint VAO() const;
		GLuint VBO() const;
		GLuint EBO() const;
		const std::vector<std::uint32_t>* indices() const;

		void connectOnMeshChanged(std::function<void(MeshID)> action) const;

	private:
		MeshID m_id;
		GLuint m_VAO; // Vertex array object.
		GLuint m_VBO; // Vertex buffer object.
		GLuint m_EBO; // Element buffer object.
		std::vector<Vertex> m_vertices;
		std::vector<std::uint32_t> m_indices;

		SignalEmitter<void(MeshID)> m_onMeshChanged;
	};
} // namespace engine