#pragma once

namespace engine
{
	namespace executable
	{
		class IPipelineStorage;
		class Runtime;

		class RuntimeAccessor
		{
		protected:
			const Runtime* runtime();
			const IPipelineStorage* storage();

		private:
			const Runtime* m_runtime;
		};
	} // namespace executable
} // namespace engine