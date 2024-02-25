#include "engine_main.h"

#include <engine/debug/memory/memory_guard.h>
#ifdef IS_EDITOR
#include <engine/executable/editor_runtime_pipeline.h>
#else
#include <engine/executable/production_runtime_pipeline.h>
#endif // #ifdef IS_EDITOR
#include <engine/executable/runtime.h>

namespace engine
{
	int Main::run()
	{
		MEMORY_GUARD;

		Runtime runtime;
#ifdef IS_EDITOR
		runtime.setPipeline(new EditorRuntimePipeline());
#else
		runtime.setPipeline(new ProductionRuntimePipeline());
#endif // #ifdef IS_EDITOR
		if (runtime.initialize())
		{
			runtime.run();
			return 0;
		}
		else
		{
			return -1;
		}
	}
} // namespace engine
