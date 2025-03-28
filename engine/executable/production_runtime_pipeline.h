#pragma once

#include <memory>

#include <engine/core/time/scoped_time.h>
#include <engine/executable/i_runtime_pipeline.h>

namespace engine
{
	class ComponentRegistrar;
	class EntityManager;
	class EntityManagerAccessor;
	class IGraphicAPI;
	class InputSystem;
	class InputSystemAccessor;
	class IRenderPipeline;
	class Logger;
	class ResourceManager;
	class TimeManager;
	class WindowManager;
	class ProductionRuntimePipeline : public IRuntimePipeline
	{
		// friends
	private:
		friend class Main;

		// members
	protected:
		explicit ProductionRuntimePipeline() = default;

		virtual bool initialize() override;
		virtual void finalize() override;
		virtual void run() override;

#if USE_LOGGER
		[[nodiscard]] bool initializeLogger();
#endif // #if USE_LOGGER
		[[nodiscard]] bool initializeGLFW();
		[[nodiscard]] bool initializeInputSystem();
		[[nodiscard]] bool initializeWindowManager();
		[[nodiscard]] bool initializeGraphicAPI();
		[[nodiscard]] bool initializeTimeManager();
		[[nodiscard]] bool initializeComponentRegistrar();
		[[nodiscard]] bool initializeEntityManager();
		[[nodiscard]] bool initializeResourceManager();

		[[nodiscard]] bool registerBuiltinComponents();
		[[nodiscard]] bool registerBuiltinResources();

		[[nodiscard]] virtual bool initializeRenderPipeline();

		void finalizeGLFW();
		void finalizeGraphicAPI();

		[[nodiscard]] ScopedTime startDeltaTimer();

		std::shared_ptr<Logger> m_logger;
		std::shared_ptr<WindowManager> m_windowManager;
		std::shared_ptr<IGraphicAPI> m_graphicAPI;
		std::shared_ptr<TimeManager> m_timeManager;
		std::shared_ptr<ComponentRegistrar> m_componentRegistrar;
		std::shared_ptr<EntityManager> m_entityManager;
		std::shared_ptr<EntityManagerAccessor> m_entityManagerAccessor;
		std::shared_ptr<ResourceManager> m_resourceManager;
		std::shared_ptr<IRenderPipeline> m_renderPipeline;
		std::shared_ptr<InputSystem> m_inputSystem;
		std::shared_ptr<InputSystemAccessor> m_inputSystemAccessor;
	};
} // namespace engine
