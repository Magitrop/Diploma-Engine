#pragma once

#include <memory>

#include <engine/core/components/mesh_renderer_component.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/internal/render/glad/glad_graphic_api.h>
#include <engine/internal/render/glad/glad_material.h>
#include <engine/internal/render/glad/glad_mesh.h>

namespace engine
{
	class EntityManager;
	class ResourceManager;
	class Transform;
	class GladMeshRenderer final : public MeshRenderer
	{
		// friends
	private:
		friend class GladRenderPipeline;
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;

		// members
	public:
		explicit GladMeshRenderer(std::shared_ptr<ResourceManager> resourceManager,
								  std::shared_ptr<EntityManager> entityManager);
		virtual ~GladMeshRenderer() override {};

		virtual void setMaterial(ComponentID componentID, MaterialID materialID) override;
		virtual void setMesh(ComponentID componentID, MeshID meshID) override;

		void updateMeshesWithID(MeshID id);

	private:
		void drawMeshes(const Matrix4x4& projection, const Matrix4x4& view, bool selectionFramebuffer);
	};
} // namespace engine
