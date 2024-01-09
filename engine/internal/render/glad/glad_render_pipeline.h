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
		virtual bool initialize() override;
		virtual void finalize() override;

		virtual void clearFrame() override;
		virtual void renderFrame(std::shared_ptr<IRenderContext> renderContext) override;

		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<GladResourceManager> m_resourceManager;
		std::shared_ptr<GladMeshRendererInternal> m_meshRenderer;
	};
} // namespace engine