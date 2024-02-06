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
										std::shared_ptr<IEditorDrawer> editorDrawer)
	{
		bool result = true;

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
			}, 0
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
				GladFramebuffer* selectionFramebuffer = static_cast<GladFramebuffer*>(window.framebuffers().viewportFramebuffer.get());
				ScopedFramebuffer currentFramebuffer = selectionFramebuffer->useFramebuffer();
				glViewport(0, 0, selectionFramebuffer->width(), selectionFramebuffer->height());
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				drawEditorContext(projectionMatrix, viewMatrix);
				drawMeshes(projectionMatrix, viewMatrix);
			}

			{
				GladFramebuffer* selectionFramebuffer = static_cast<GladFramebuffer*>(window.framebuffers().selectionFramebuffer.get());
				ScopedFramebuffer currentFramebuffer = selectionFramebuffer->useFramebuffer();
				glViewport(0, 0, selectionFramebuffer->width(), selectionFramebuffer->height());
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//drawEditorContext(projectionMatrix, viewMatrix);
			}
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void GladRenderPipeline::renderEditorSimulation()
	{
		MEMORY_GUARD;
	}

	void GladRenderPipeline::drawMeshes(const Matrix4x4& projection, const Matrix4x4& view)
	{
		MEMORY_GUARD;

		std::size_t count = m_meshRenderer->m_meshID.size();

		const auto materialIt = m_meshRenderer->m_material.begin();
		const auto VAOIt = m_meshRenderer->m_meshVAO.begin();
		const auto indicesIt = m_meshRenderer->m_meshIndices.begin();

		Matrix4x4 modelMatrix = Matrix4x4(1);
		// TODO: realize material & shader sorting and corresponding rendering
		for (std::size_t i = 0; i < count; ++i, ++materialIt, ++VAOIt, ++indicesIt)
		{
			const GladMaterialImpl* material = materialIt->get();
			const GladShader* shader = material->shader();
			const GLuint VAO = VAOIt->get();
			const auto& indices = indicesIt->get();

			material->useShader();

			shader->setMatrix4x4("projection", projection);
			shader->setMatrix4x4("view", view);
			shader->setMatrix4x4("model", modelMatrix);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, &(*indices)[0]);
		}
	}

	void GladRenderPipeline::drawEditorContext(const Matrix4x4& projection, const Matrix4x4& view)
	{
		MEMORY_GUARD;

		auto context = std::static_pointer_cast<GladEditorDrawerContext>(m_editorDrawer->context());
		DEBUG_ASSERT(context != nullptr);

		const auto& primitives = context->primitives();
		if (primitives.empty())
			return;

		std::int8_t renderQueueIndex = 0;
		for (auto& primitive : primitives)
		{
			const GladMaterialImpl* material = m_resourceManager->getMaterial(primitive.material);
			const GladShader* shader = material->shader();

			if (material->renderQueue() != renderQueueIndex)
			{
				renderQueueIndex = material->renderQueue();
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			const GLuint VAO = primitive.VAO;
			const auto* indices = &primitive.indices;

			material->useShader();

			shader->setMatrix4x4("projection", projection);
			shader->setMatrix4x4("view", view);
			shader->setMatrix4x4("model", primitive.transform);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, &(*indices)[0]);
		}
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