#pragma once

namespace engine
{
	class IGraphicAPI
	{
		friend class ProductionRuntimePipeline;

	protected:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;
	};
} // namespace engine