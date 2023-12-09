#pragma once

#include <memory>

#include <engine/internal/render/graphics/i_render_pipeline.h>

namespace engine
{
	class EntityManager;
	class GladRenderPipeline final : public IRenderPipeline
	{
	private:
		explicit GladRenderPipeline(std::shared_ptr<EntityManager> entityManager);

	public:
		~GladRenderPipeline();

	private:
		virtual bool initialize() override;
		virtual void finalize() override;

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine