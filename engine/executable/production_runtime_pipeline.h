#pragma once

#include <engine/executable/i_runtime_pipeline.h>

#include <memory>

namespace engine
{
	namespace render
	{
		class Window;
	} // namespace render

	namespace internal
	{
		namespace render
		{
			class WindowManager;
		} // namespace render
	} // namespace internal

	namespace executable
	{
		class ProductionRuntimePipeline : public IRuntimePipeline
		{
			// usings
		protected:
			using WindowManager = engine::internal::render::WindowManager;
			using Window = engine::render::Window;

			// friends
		private:
			friend class Runtime;
			friend class RuntimeAccessor;

			// members
		protected:
			virtual void initialize() override;
			virtual void run() override;

			void initializeGLFW();
			void initializeGraphicAPI();

			void initializeWindowManager();

			std::shared_ptr<WindowManager> m_windowManager;
		};
	}
}
