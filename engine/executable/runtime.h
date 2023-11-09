#pragma once

#include <memory>

int main();

namespace engine
{
	namespace executable
	{
		class RuntimePipeline;

		class Runtime final
		{
			// friends
			friend int ::main();

			// members
		private:
			explicit Runtime();
			Runtime(Runtime&) = delete;
			Runtime(Runtime&&) = delete;
			Runtime& operator = (Runtime&) = delete;
			Runtime& operator = (Runtime&&) = delete;

			void initialize();

		public:
			std::shared_ptr<const RuntimePipeline> getPipeline() const;

		private:
			template<typename Pipeline>
			void setPipeline()
			{
				m_runtimePipeline = Pipeline::Constructor::constructShared();
			}

			std::shared_ptr<RuntimePipeline> m_runtimePipeline;
		};
	} // namespace executable
} // namespace engine
