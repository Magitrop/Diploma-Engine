#include "glad_render_pipeline.h"

#include <engine/core/entity/entity_manager.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_framebuffer.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component_impl.h>
#include <engine/internal/render/glad/glad_resource_manager.h>
#if IS_EDITOR
#include <engine/editor/viewport/editor_camera.h>
#endif // #if IS_EDITOR

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

#if IS_EDITOR
	bool GladRenderPipeline::initialize(std::shared_ptr<EditorViewports> viewports)
	{
		bool result = true;

		m_editorViewports = std::move(viewports);
		result &= m_editorViewports != nullptr;

		return result;
	}

	std::shared_ptr<IFramebuffer> GladRenderPipeline::createFramebuffer(std::uint32_t width,
																		std::uint32_t height)
	{
		return std::shared_ptr<IFramebuffer>(new GladFramebuffer(width, height));
	}

	void GladRenderPipeline::renderEditorViewports()
	{
		MEMORY_GUARD;

		EditorViewports viewports = *m_editorViewports.get();
		for (const auto& camera : viewports)
		{
			GladFramebuffer* framebuffer = static_cast<GladFramebuffer*>(camera.framebuffer.get());
			auto f = framebuffer->useFramebuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void GladRenderPipeline::renderEditorSimulation()
	{

	}
#else // #if IS_EDITOR
	void GladRenderPipeline::renderFrame()
	{
		MEMORY_GUARD;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (std::size_t contextIdx = 0; contextIdx < m_framebuffer.size(); ++contextIdx)
		{
			
		}

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
#endif // #if IS_EDITOR

	void GladRenderPipeline::finalize()
	{
	}
} // namespace engine