#include "glad_render_pipeline.h"

#include <engine/core/entity/entity_manager.h>
#include <engine/internal/render/graphics/glad/glad_render_component.h>

namespace engine
{
	class GladRenderPipeline::Internal final
	{
	public:
		Internal(std::shared_ptr<EntityManager> entityManager);
		~Internal() = default;

		bool initialize();
		void finalize();

	private:
		std::shared_ptr<GladRenderComponent> m_renderManager;
		std::shared_ptr<EntityManager> m_entityManager;
	};

	GladRenderPipeline::Internal::Internal(std::shared_ptr<EntityManager> entityManager)
		: m_entityManager(entityManager)
	{
		m_renderManager = m_entityManager->getComponentManager<GladRenderComponent>();
	}

	bool GladRenderPipeline::Internal::initialize()
	{
		return true;
	}

	void GladRenderPipeline::Internal::finalize()
	{}

	GladRenderPipeline::GladRenderPipeline(std::shared_ptr<EntityManager> entityManager)
	{
		m_internal = std::make_unique<Internal>(entityManager);
	}

	GladRenderPipeline::~GladRenderPipeline()
	{}

	bool GladRenderPipeline::initialize()
	{
		return m_internal->initialize();
	}

	void GladRenderPipeline::finalize()
	{
		m_internal->finalize();
	}
} // namespace engine