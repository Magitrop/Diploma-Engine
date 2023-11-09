#pragma once

namespace engine
{
	namespace executable
	{
		class IPipelineStorage
		{

		};

		class IRuntimePipeline
		{
			friend class RuntimeAccessor;
			virtual const IPipelineStorage* storage() const = 0;
		};
	} // namespace executable
} // namespace engine
