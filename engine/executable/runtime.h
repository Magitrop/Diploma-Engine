#pragma once

#include <memory>
#include <engine/internal/helpers/non_copyable.h>
#include <engine/internal/helpers/non_movable.h>
#include <engine/executable/i_runtime_pipeline.h>

int main();

namespace engine
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
} // namespace engine
