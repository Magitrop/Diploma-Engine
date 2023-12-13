#include "engine_main.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/executable/editor_runtime_pipeline.h>
#include <engine/executable/runtime.h>

namespace engine
{
	int Main::run()
	{
		MEMORY_GUARD;

		Runtime runtime;
		runtime.setPipeline(new EditorRuntimePipeline());
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
