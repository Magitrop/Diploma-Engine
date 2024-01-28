#pragma once

#include <engine/core/components/component_internal_base.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class GladResourceManager;
	class GladMeshRendererInternal final : public ComponentInternal
	{
	private:
		friend class GladRenderPipeline;

	public:
		explicit GladMeshRendererInternal(std::shared_ptr<ResourceManager> resourceManager,
										  GladMeshRenderer* meshRenderer);
		~GladMeshRendererInternal() = default;

		virtual void attachComponent(ComponentID component) override;
		virtual void detachComponent(ComponentID component) override;

		void setMaterial(ComponentID componentID, MaterialID materialID);
		void setMesh(ComponentID componentID, MeshID meshID);

		void updateMeshesWithID(MeshID id);

	private:
		PersistentVector<MeshID, constants::VERY_FREQUENT_COMPONENT> m_meshID;
		PersistentVector<GLuint, constants::VERY_FREQUENT_COMPONENT> m_meshVAO;
		PersistentVector<const std::vector<std::uint32_t>*, constants::VERY_FREQUENT_COMPONENT> m_meshIndices;
		PersistentVector<const GladMaterialImpl*, constants::VERY_FREQUENT_COMPONENT> m_material;

		std::shared_ptr<GladResourceManager> m_resourceManager;
	};
} // namespace engine