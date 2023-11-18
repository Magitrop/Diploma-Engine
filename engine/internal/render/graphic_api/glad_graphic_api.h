#pragma once

#include <engine/internal/render/graphic_api/i_graphic_api.h>

namespace engine
{
	class GladGraphicAPI final : public IGraphicAPI
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
	private:
		explicit GladGraphicAPI();

	public:
		virtual bool initialize() override;
		virtual void finalize() override {}

	private:
	};
} // namespace engine