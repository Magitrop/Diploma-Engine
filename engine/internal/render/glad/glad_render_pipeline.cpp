#include "glad_render_pipeline.h"

#include <engine/core/entity/entity_manager.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component_impl.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

#if IS_EDITOR
#include <engine/editor/gui/editor_viewport_window.h>
#include <engine/editor/editor.h>
#include <engine/internal/render/glad/glad_editor_drawer.h>
#endif // #if IS_EDITOR

#include <engine/dependencies/glm/glm/ext/matrix_clip_space.hpp>
#include <engine/dependencies/glm/glm/ext/matrix_transform.hpp>

namespace engine
{
	GladRenderPipeline::GladRenderPipeline(std::shared_ptr<EntityManager> entityManager,
										   std::shared_ptr<ResourceManager> resourceManager)
		: m_entityManager(entityManager)
		, m_resourceManager(std::dynamic_pointer_cast<GladResourceManager>(resourceManager))
		, m_meshRenderer(dynamic_cast<GladMeshRenderer*>(entityManager->getComponentManager<MeshRenderer>()))
	{
		DEBUG_ASSERT(m_entityManager != nullptr);
		DEBUG_ASSERT(m_resourceManager != nullptr);
		DEBUG_ASSERT(m_meshRenderer != nullptr);
	}

	GladRenderPipeline::~GladRenderPipeline()
	{}

#if IS_EDITOR
	bool GladRenderPipeline::initialize(std::shared_ptr<EditorViewportsManager> viewports,
										std::shared_ptr<IEditorDrawer> editorDrawer)
	{
		bool result = true;

		glLineWidth(8);

		//glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(
			[](GLenum source,
			   GLenum type,
			   GLuint id,
			   GLenum severity,
			   GLsizei length,
			   const GLchar* message,
			   const void* userParam)
			{
				if (type == GL_DEBUG_TYPE_ERROR)
					ERROR_LOG(message);
			}, nullptr
		);

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

		m_editorDrawer->beginDraw();
		for (auto& slot : m_viewports->viewports())
		{
			if (slot.empty())
				continue;
			auto& window = slot.get();
			auto& viewport = window.viewport();

			Matrix4x4 projectionMatrix = viewport.projection();
			Matrix4x4 viewMatrix = viewport.view();
			Matrix4x4 modelMatrix = Matrix4x4(1);

			{
				GladFramebuffer* viewportFramebuffer = static_cast<GladFramebuffer*>(window.framebuffers().viewportFramebuffer.get());
				ScopedFramebuffer currentFramebuffer = viewportFramebuffer->useFramebuffer();
				glViewport(0, 0, viewportFramebuffer->width(), viewportFramebuffer->height());
				glClear(GL_COLOR_BUFFER_BIT);

				m_editorDrawer->drawSpatialGrid(projectionMatrix, viewMatrix);

				glClear(GL_DEPTH_BUFFER_BIT);
				m_editorDrawer->drawContext(projectionMatrix, viewMatrix);
				m_meshRenderer->drawMeshes(projectionMatrix, viewMatrix, false);

				glClear(GL_DEPTH_BUFFER_BIT);
				m_editorDrawer->drawGizmo(projectionMatrix, viewMatrix, false);
			}

			{
				GladFramebuffer* selectionFramebuffer = static_cast<GladFramebuffer*>(window.framebuffers().selectionFramebuffer.get());
				ScopedFramebuffer currentFramebuffer = selectionFramebuffer->useFramebuffer();
				glViewport(0, 0, selectionFramebuffer->width(), selectionFramebuffer->height());
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
				glDisable(GL_BLEND);

				m_meshRenderer->drawMeshes(projectionMatrix, viewMatrix, true);
				glClear(GL_DEPTH_BUFFER_BIT);
				m_editorDrawer->drawGizmo(projectionMatrix, viewMatrix, true);

				glEnable(GL_BLEND);
			}
		}
		m_editorDrawer->endDraw();

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