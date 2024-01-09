#include "glad_mesh_renderer_component.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_resource_manager.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component_impl.h>

namespace engine
{
	GladMeshRenderer::GladMeshRenderer(std::shared_ptr<ResourceManager> resourceManager)
	{
		m_internal = std::unique_ptr<GladMeshRendererInternal>(new GladMeshRendererInternal(resourceManager, this));
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
} // namespace engine