#include "glad_render_pipeline.h"

#include <engine/core/entity/entity_manager.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_framebuffer.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component_impl.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

#if IS_EDITOR
#include <engine/editor/gui/editor_viewport_window.h>
#include <engine/editor/editor.h>
#include <engine/editor/gizmo/editor_drawer.h>
#endif // #if IS_EDITOR

#include <engine/dependencies/glm/glm/ext/matrix_clip_space.hpp>
#include <engine/dependencies/glm/glm/ext/matrix_transform.hpp>

namespace engine
{
	GladRenderPipeline::GladRenderPipeline(std::shared_ptr<EntityManager> entityManager,
										   std::shared_ptr<ResourceManager> resourceManager)
		: m_entityManager(entityManager)
		, m_resourceManager(std::dynamic_pointer_cast<GladResourceManager>(resourceManager))
	{
		DEBUG_ASSERT(m_resourceManager != nullptr);

		auto renderer = std::dynamic_pointer_cast<GladMeshRenderer>(entityManager->getComponentManager<MeshRenderer>());
		DEBUG_ASSERT(renderer != nullptr);
		m_meshRenderer = std::dynamic_pointer_cast<GladMeshRendererInternal>(renderer->m_internal);
		DEBUG_ASSERT(m_meshRenderer != nullptr);
	}

	GladRenderPipeline::~GladRenderPipeline()
	{}

#if IS_EDITOR
	bool GladRenderPipeline::initialize(std::shared_ptr<EditorViewportsManager> viewports,
										std::shared_ptr<EditorDrawer> editorDrawer)
	{
		bool result = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_editorDrawer = std::move(editorDrawer);
		DEBUG_ASSERT(m_editorDrawer != nullptr);

		m_viewports = std::move(viewports);
		DEBUG_ASSERT(m_viewports != nullptr);

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

		for (auto& slot : m_viewports->viewports())
		{
			if (slot.empty())
				continue;
			auto& window = slot.get();
			auto& viewport = window.viewport();

			GladFramebuffer* framebuffer = static_cast<GladFramebuffer*>(window.framebuffer().get());
			ScopedFramebuffer scopedFramebuffer = framebuffer->useFramebuffer();
			glViewport(0, 0, framebuffer->width(), framebuffer->height());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Matrix4x4 projectionMatrix = viewport.projection();
			Matrix4x4 viewMatrix = viewport.view();
			Matrix4x4 modelMatrix = Matrix4x4(1);

			// TODO: realize material & shader sorting and corresponding rendering
			for (std::size_t i = 0; i < m_meshRenderer->m_meshID.size(); ++i)
			{
				const GladMaterialImpl* material = m_meshRenderer->m_material.at(i)->get();
				GladShader* shader = material->m_shader;
				GLuint VAO = m_meshRenderer->m_meshVAO.at(i)->get();
				auto* indices = m_meshRenderer->m_meshIndices.at(i)->get();

				material->useShader();

				shader->setMatrix4x4("projection", projectionMatrix);
				shader->setMatrix4x4("view", viewMatrix);
				shader->setMatrix4x4("model", modelMatrix);

				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, &(*indices)[0]);
			}

			auto editorDrawerContext(std::move(m_editorDrawer->obtainContext()));
			for (auto& primitive : editorDrawerContext.m_primitives)
			{

			}
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void GladRenderPipeline::renderEditorSimulation()
	{
		MEMORY_GUARD;
	}

#else // #if IS_EDITOR
	void GladRenderPipeline::renderFrame()
	{
		MEMORY_GUARD;
	}
#endif // #if IS_EDITOR

	void GladRenderPipeline::finalize()
	{
	}
} // namespace engine