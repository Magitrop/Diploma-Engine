#pragma once

#include <memory>

#include <engine/core/math/matrix4x4.h>
#include <engine/internal/render/glad/glad_framebuffer.h>
#include <engine/internal/render/i_render_pipeline.h>

namespace engine
{
	class EntityManager;
	class GladMeshRenderer;
	class GladResourceManager;
	class Renderer;
	class ResourceManager;
	class GladRenderPipeline final : public IRenderPipeline
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		explicit GladRenderPipeline(std::shared_ptr<EntityManager> entityManager,
									std::shared_ptr<ResourceManager> resourceManager);

	public:
		~GladRenderPipeline();

#if IS_EDITOR
	private:
		virtual bool initialize(std::shared_ptr<EditorViewportsManager> viewports,
								std::shared_ptr<IEditorDrawer> editorDrawer) override;

		virtual void renderEditorViewports() override;
		virtual void renderEditorSimulation() override;

	public:
		virtual std::shared_ptr<IFramebuffer> createFramebuffer(std::uint32_t width,
																std::uint32_t height) override;
#else
	private:
		virtual void renderFrame() override;
#endif  // #if IS_EDITOR
	private:
		virtual void finalize() override;

	private:
		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<GladResourceManager> m_resourceManager;
		std::shared_ptr<GladMeshRenderer> m_meshRenderer;

#if IS_EDITOR
		std::shared_ptr<EditorViewportsManager> m_viewports;
		std::shared_ptr<IEditorDrawer> m_editorDrawer;
#endif  // #if IS_EDITOR
	};
} // namespace engine