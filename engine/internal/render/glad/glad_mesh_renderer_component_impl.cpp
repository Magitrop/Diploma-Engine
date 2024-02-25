#include "glad_mesh_renderer_component_impl.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>

#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

#ifdef IS_EDITOR
#include <engine/editor/editor_selection.h>
#endif // #ifdef IS_EDITOR

namespace engine
{
	GladMeshRendererInternal::GladMeshRendererInternal(std::shared_ptr<ResourceManager> resourceManager,
													   std::shared_ptr<EntityManager> entityManager,
													   GladMeshRenderer* meshRenderer)
		: m_resourceManager(std::dynamic_pointer_cast<GladResourceManager>(resourceManager))
		, m_entityManager(std::move(entityManager))
	{
		DEBUG_ASSERT(m_resourceManager != nullptr);
		DEBUG_ASSERT(m_entityManager != nullptr);
		m_resourceManager->setMeshRenderer(std::shared_ptr<GladMeshRenderer>(meshRenderer));
	}

	void GladMeshRendererInternal::attachComponent(EntityID entity, ComponentID component) /* override */
	{
		MEMORY_GUARD;

		MeshData data
		{
			entity,
			GLuint(),
			nullptr,
			m_resourceManager->getDefaultMaterial()
		};

		m_meshData.push(data);
		m_meshID.push(MeshID());
	}

	void GladMeshRendererInternal::detachComponent(EntityID entity, ComponentID component) /* override */
	{
		MEMORY_GUARD;

		m_meshID.remove(m_meshID.begin() + component);
		m_meshData.remove(m_meshData.begin() + component);
	}

	void GladMeshRendererInternal::setMaterial(ComponentID componentID, MaterialID materialID)
	{
		MEMORY_GUARD;

		m_meshData.at(componentID)->get().material = m_resourceManager->getMaterial(materialID);
	}

	void GladMeshRendererInternal::setMesh(ComponentID componentID, MeshID meshID)
	{
		MEMORY_GUARD;

		GladMeshImpl* mesh = m_resourceManager->getMeshImpl(meshID);
		if (!mesh)
			return;

		m_meshID.at(componentID)->get() = meshID;
		MeshData& data = m_meshData.at(componentID)->get();

		data.VAO = mesh->VAO();
		data.indices = mesh->indices();
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

		const auto& meshIDIt = m_meshID.begin();
		for (auto& slot : m_meshData)
		{
			if (slot.empty())
				continue;

			if (id == meshIDIt->get())
			{
				MeshData& data = slot.get();
				data.VAO = mesh->VAO();
				data.indices = mesh->indices();
			}
			++meshIDIt;
		}
	}

	void GladMeshRendererInternal::drawMeshes(const Matrix4x4& projection, const Matrix4x4& view, bool selectionFramebuffer)
	{
		std::size_t count = m_meshData.size();
		const auto& meshIt = m_meshData.begin();
		const auto& meshIDIt = m_meshID.begin();

#ifdef IS_EDITOR
		static const GladMaterialImpl* selectionMaterial = m_resourceManager->getMaterial("selection");
#endif // #ifdef IS_EDITOR

		Transform* transformManager = m_entityManager->getComponentManager<Transform>();

		// TODO: realize material & shader sorting and corresponding rendering
		for (std::size_t i = 0; i < count; ++i, ++meshIt, ++meshIDIt)
		{
			if (meshIt->empty() || meshIDIt->empty())
				continue;

			const auto& [owner, VAO, indices, _] = meshIt->get();
			if (meshIDIt->get() == MeshID() ||
				indices->size() == 0)
				continue;

			ComponentID transformID = m_entityManager->getComponent<Transform>(owner);
			// TODO: get the model matrix vector directly (i.e. transformManager->m_internal->m_modelMatrix)
			Matrix4x4 model = transformManager->modelMatrix(transformID);

			const GladMaterialImpl* material = meshIt->get().material;
#ifdef IS_EDITOR
			if (selectionFramebuffer)
				material = selectionMaterial;
#endif // #ifdef IS_EDITOR

			const GladShader* shader = material->shader();

			material->useShader();

			shader->setMatrix4x4("projection", projection);
			shader->setMatrix4x4("view", view);
			shader->setMatrix4x4("model", model);
#ifdef IS_EDITOR
			shader->setVector4("selectionColor", EditorSelectionManager::entityToColor(owner));
#endif // #ifdef IS_EDITOR

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, &(*indices)[0]);
		}
	}
} // namespace engine