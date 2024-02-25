#include "glad_mesh_renderer_component.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_resource_manager.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component_impl.h>

namespace engine
{
	GladMeshRenderer::GladMeshRenderer(std::shared_ptr<ResourceManager> resourceManager,
									   std::shared_ptr<EntityManager> entityManager)
	{
		m_internal = std::shared_ptr<GladMeshRendererInternal>(
			new GladMeshRendererInternal(
				std::move(resourceManager),
				std::move(entityManager),
				this
			));
	}

	void GladMeshRenderer::setMaterial(ComponentID componentID, MaterialID materialID)
	{
		static_cast<GladMeshRendererInternal*>(m_internal.get())->setMaterial(componentID, materialID);
	}

	void GladMeshRenderer::setMesh(ComponentID componentID, MeshID meshID)
	{
		static_cast<GladMeshRendererInternal*>(m_internal.get())->setMesh(componentID, meshID);
	}

	void GladMeshRenderer::updateMeshesWithID(MeshID id)
	{
		static_cast<GladMeshRendererInternal*>(m_internal.get())->updateMeshesWithID(id);
	}

	void GladMeshRenderer::drawMeshes(const Matrix4x4& projection, const Matrix4x4& view, bool selectionFramebuffer)
	{
		static_cast<GladMeshRendererInternal*>(m_internal.get())->drawMeshes(projection, view, selectionFramebuffer);
	}
} // namespace engine