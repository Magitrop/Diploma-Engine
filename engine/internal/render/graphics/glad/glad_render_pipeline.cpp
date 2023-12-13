#include "glad_render_pipeline.h"

#include <engine/core/entity/entity_manager.h>
#include <engine/internal/render/graphics/glad/glad_render_component.h>

namespace engine
{
	GladRenderPipeline::GladRenderPipeline(std::shared_ptr<EntityManager> entityManager,
										   std::shared_ptr<ResourceManager> resourceManager,
										   std::shared_ptr<WindowManager> windowManager)
		: m_entityManager(entityManager)
		, m_resourceManager(resourceManager)
		, m_windowManager(windowManager)
	{

	}

	GladRenderPipeline::~GladRenderPipeline()
	{}

	bool GladRenderPipeline::initialize()
	{
		return true;
	}

	void GladRenderPipeline::finalize()
	{
	}

	void GladRenderPipeline::renderFrame()
	{
	}
} // namespace engine