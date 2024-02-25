#pragma once

#include <engine/core/components/component_internal_base.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class GladResourceManager;
	class EntityManager;
	class Transform;
	class GladMeshRendererInternal final : public ComponentInternal
	{
	private:
		friend class GladMeshRenderer;

	public:
		explicit GladMeshRendererInternal(std::shared_ptr<ResourceManager> resourceManager,
										  std::shared_ptr<EntityManager> entityManager,
										  GladMeshRenderer* meshRenderer);
		~GladMeshRendererInternal() = default;

		virtual void attachComponent(EntityID entity, ComponentID component) override;
		virtual void detachComponent(EntityID entity, ComponentID component) override;

		void setMaterial(ComponentID componentID, MaterialID materialID);
		void setMesh(ComponentID componentID, MeshID meshID);

		void updateMeshesWithID(MeshID id);

	private:
		void drawMeshes(const Matrix4x4& projection, const Matrix4x4& view, bool selectionFramebuffer);

	private:
		struct MeshData
		{
			EntityID owner;
			GLuint VAO;
			const std::vector<std::uint32_t>* indices;
			const GladMaterialImpl* material;
		};
		PersistentVector<MeshData, constants::VERY_FREQUENT_COMPONENT> m_meshData;
		PersistentVector<MeshID, constants::VERY_FREQUENT_COMPONENT> m_meshID;

		std::shared_ptr<GladResourceManager> m_resourceManager;
		std::shared_ptr<Transform> m_transformManager;
		std::shared_ptr<EntityManager> m_entityManager;
	};
} // namespace engine