#pragma once

#include <memory>

namespace engine
{
	class IRenderContext;
	class IRenderPipeline
	{
		// friends
	private:
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;
		friend class ScopedFrame;

		// members
	protected:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;

		virtual void clearFrame() = 0;
		virtual void renderFrame(std::shared_ptr<IRenderContext> renderContext) = 0;
	};
} // namespace engine
