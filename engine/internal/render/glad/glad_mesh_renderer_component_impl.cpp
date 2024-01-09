#include "glad_mesh_renderer_component_impl.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

namespace engine
{
	GladMeshRendererInternal::GladMeshRendererInternal(std::shared_ptr<ResourceManager> resourceManager,
													   GladMeshRenderer* meshRenderer)
		: m_resourceManager(std::dynamic_pointer_cast<GladResourceManager>(resourceManager))
	{
		DEBUG_ASSERT(m_resourceManager != nullptr);
		m_resourceManager->setMeshRenderer(std::shared_ptr<GladMeshRenderer>(meshRenderer));
	}

	void GladMeshRendererInternal::attachComponent(ComponentID component) /* override */
	{
		MEMORY_GUARD;

		m_meshID.push(MeshID());
		m_meshVAO.push(GLuint());
		m_meshVBO.push(GLuint());
		m_material.push(m_resourceManager->getDefaultMaterial());
	}

	void GladMeshRendererInternal::detachComponent(ComponentID component) /* override */
	{
		MEMORY_GUARD;

		m_meshID.remove(m_meshID.begin() + component);
		m_meshVAO.remove(m_meshVAO.begin() + component);
		m_meshVBO.remove(m_meshVBO.begin() + component);
		m_material.remove(m_material.begin() + component);
	}

	void GladMeshRendererInternal::setMaterial(ComponentID componentID, MaterialID materialID)
	{
		MEMORY_GUARD;

		m_material.at(componentID)->get() = m_resourceManager->getMaterial(materialID);
	}

	void GladMeshRendererInternal::setMesh(ComponentID componentID, MeshID meshID)
	{
		MEMORY_GUARD;

		GladMeshImpl* mesh = m_resourceManager->getMeshImpl(meshID);
		if (!mesh)
			return;

		m_meshID.at(componentID)->get() = meshID;
		m_meshVAO.at(componentID)->get() = mesh->VAO();
		m_meshVBO.at(componentID)->get() = mesh->VBO();
	}

	void GladMeshRendererInternal::updateMeshesWithID(MeshID id)
	{
		MEMORY_GUARD;

		GladMeshImpl* mesh = m_resourceManager->getMeshImpl(id);
		if (!mesh)
		{
			ERROR_LOG("Cannot update meshes with ID {}!", id.id());
			return;
		}

		for (std::size_t i = 0; i < m_meshID.size(); ++i)
		{
			if (id == m_meshID.at(i)->get())
			{
				m_meshVAO.at(i)->get() = mesh->VAO();
				m_meshVBO.at(i)->get() = mesh->VBO();
			}
		}
	}
} // namespace engine