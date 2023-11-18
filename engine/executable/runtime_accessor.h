#pragma once

#include <memory>

namespace engine
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
} // namespace engine