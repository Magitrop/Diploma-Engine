#pragma once

#include <memory>

#include <engine/internal/render/graphics/i_graphic_api.h>

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
		~GladGraphicAPI();

	private:
		virtual bool initialize() override;
		virtual void finalize() override;

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine