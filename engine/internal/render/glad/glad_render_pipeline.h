#pragma once

#include <memory>

#include <engine/internal/render/i_render_pipeline.h>

namespace engine
{
	class EntityManager;
	class GladMeshRendererInternal;
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

	private:
#if IS_EDITOR
		virtual bool initialize(std::shared_ptr<EditorViewports> viewports) override;
		virtual std::shared_ptr<IFramebuffer> createFramebuffer(std::uint32_t width,
																std::uint32_t height) override;
		virtual void renderEditorViewports() override;
		virtual void renderEditorSimulation() override;
#else
		virtual void renderFrame() override;
#endif  // #if IS_EDITOR

		virtual void finalize() override;

		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<GladResourceManager> m_resourceManager;
		std::shared_ptr<GladMeshRendererInternal> m_meshRenderer;
	};
} // namespace engine