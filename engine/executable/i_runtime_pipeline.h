#pragma once

#include <memory>

namespace engine
{
	namespace executable
	{
		class IRuntimePipeline
		{
			// friends
		private:
			friend class Runtime;

			// members
		public:
			bool isRunning();

		protected:
			virtual void initialize() = 0;
			virtual void run() = 0;

			bool m_isRunning = false;
		};
	} // namespace executable
} // namespace engine
