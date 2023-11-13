#pragma once

#include <memory>

namespace engine
{
	namespace executable
	{
		class PipelineStorage;
		class Runtime;

		class RuntimeAccessor
		{
		protected:
			// getResourceManager()... etc

		private:
			const Runtime* m_runtime;
		};
	} // namespace executable
} // namespace engine