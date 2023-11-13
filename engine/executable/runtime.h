#pragma once

#include <memory>
#include <engine/internal/helpers/helpers/non_copyable.h>
#include <engine/internal/helpers/helpers/non_movable.h>

int main();

namespace engine
{
	namespace executable
	{
		class IRuntimePipeline;

		class Runtime final : NonCopyable, NonMovable
		{
			// friends
		private:
			friend int ::main();

			// members
		private:
			explicit Runtime();

		public:
			bool isRunning() const;

		private:
			void initialize();
			void run();

			template<typename Pipeline>
			void setPipeline()
			{
				m_runtimePipeline = std::make_shared<Pipeline>(std::move(Pipeline()));
			}

			std::shared_ptr<IRuntimePipeline> m_runtimePipeline;
		};
	} // namespace executable
} // namespace engine
