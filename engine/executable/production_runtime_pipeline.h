#pragma once

#include <engine/executable/i_runtime_pipeline.h>
#include <engine/internal/render/graphic_api/i_graphic_api.h>

#include <memory>

namespace engine
{
	class WindowManager;
	class ProductionRuntimePipeline : public IRuntimePipeline
	{
		// friends
	private:
		friend class Runtime;
		friend class RuntimeAccessor;

		// members
	protected:
		virtual void initialize() override;
		virtual void finalize() override;
		virtual void run() override;

		void initializeGLFW();
		void initializeGraphicAPI();
		void initializeWindowManager();

		void finalizeGLFW();
		void finalizeGraphicAPI();

		std::shared_ptr<IGraphicAPI> m_graphicAPI;
		std::shared_ptr<WindowManager> m_windowManager;
	};
} // namespace engine
