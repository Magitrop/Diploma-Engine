#pragma once

#include <engine/core/time/scoped_time.h>
#include <engine/executable/i_runtime_pipeline.h>
#include <engine/internal/render/graphic_api/i_graphic_api.h>

#include <memory>

namespace engine
{
	class ComponentsManager;
	class Logger;
	class ObjectsManager;
	class TimeManager;
	class WindowManager;
	class ProductionRuntimePipeline : public IRuntimePipeline
	{
		// friends
	private:
		friend class Runtime;
		friend class RuntimeAccessor;

		// members
	protected:
		virtual bool initialize() override;
		virtual void finalize() override;
		virtual void run() override;

#if USE_LOGGER
		bool initializeLogger();
#endif // #if USE_LOGGER
		bool initializeGLFW();
		bool initializeGraphicAPI();
		bool initializeWindowManager();
		bool initializeTimeManager();
		bool initializeObjectsManager();
		bool initializeComponentsManager();

		void finalizeGLFW();
		void finalizeGraphicAPI();

		[[nodiscard]] ScopedTime startDeltaTimer();

		std::shared_ptr<IGraphicAPI> m_graphicAPI;
		std::shared_ptr<WindowManager> m_windowManager;
		std::shared_ptr<TimeManager> m_timeManager;
		std::shared_ptr<ObjectsManager> m_objectsManager;
		std::shared_ptr<ComponentsManager> m_componentsManager;
		std::shared_ptr<Logger> m_logger;
	};
} // namespace engine
