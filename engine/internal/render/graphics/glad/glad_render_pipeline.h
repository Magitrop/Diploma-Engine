#pragma once

#include <memory>

#include <engine/internal/render/graphics/i_render_pipeline.h>

namespace engine
{
	class EntityManager;
	class ResourceManager;
	class WindowManager;
	class GladRenderPipeline final : public IRenderPipeline
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		explicit GladRenderPipeline(std::shared_ptr<EntityManager> entityManager,
									std::shared_ptr<ResourceManager> resourceManager,
									std::shared_ptr<WindowManager> windowManager);

	public:
		~GladRenderPipeline();

	private:
		virtual bool initialize() override;
		virtual void finalize() override;

		virtual void renderFrame() override;

		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<ResourceManager> m_resourceManager;
		std::shared_ptr<WindowManager> m_windowManager;
	};
} // namespace engine