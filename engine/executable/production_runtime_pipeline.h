#pragma once

#include <engine/internal/helpers/helpers/construction_helper.h>

#include <memory>

int main();

namespace engine
{
	namespace internal
	{
		namespace render
		{
			class WindowManager;
		} // namespace render
	} // namespace internal

	namespace executable
	{
		class ProductionRuntimePipeline final
		{
			friend int ::main();

			using WindowManager = engine::internal::render::WindowManager;

		private:
			explicit ProductionRuntimePipeline() = default;

		public:
			//virtual void initialize() override;
			//virtual void runtime() override;
		};
	}
}
