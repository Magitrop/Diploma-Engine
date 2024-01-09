#include "glad_render_pipeline.h"

#include <engine/core/entity/entity_manager.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component_impl.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

namespace engine
{
	GladRenderPipeline::GladRenderPipeline(std::shared_ptr<EntityManager> entityManager,
										   std::shared_ptr<ResourceManager> resourceManager)
		: m_entityManager(entityManager)
		, m_resourceManager(std::dynamic_pointer_cast<GladResourceManager>(resourceManager))
	{
		DEBUG_ASSERT(m_resourceManager != nullptr);

		auto renderer = std::dynamic_pointer_cast<GladMeshRenderer>(entityManager->getComponentManager<Renderer>());
		DEBUG_ASSERT(renderer != nullptr);
		m_meshRenderer = std::dynamic_pointer_cast<GladMeshRendererInternal>(renderer->m_internal);
		DEBUG_ASSERT(m_meshRenderer != nullptr);
	}

	GladRenderPipeline::~GladRenderPipeline()
	{}

	bool GladRenderPipeline::initialize()
	{
		return true;
	}

	void GladRenderPipeline::finalize()
	{
	}

	void GladRenderPipeline::clearFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GladRenderPipeline::renderFrame(std::shared_ptr<IRenderContext> renderContext)
	{
		MEMORY_GUARD;

		// TODO: realize material & shader sorting and corresponding rendering
		for (std::size_t i = 0; i < m_meshRenderer->m_meshID.size(); ++i)
		{
			const GladMaterialImpl* material = m_meshRenderer->m_material.at(i)->get();
			GladShader* shader = material->m_shader;
			GLuint VAO = m_meshRenderer->m_meshVAO.at(i)->get();

			material->useShader();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
} // namespace engine