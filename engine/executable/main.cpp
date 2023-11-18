#include <engine/debug/memory/memory_guard.h>
#include <engine/executable/editor_runtime_pipeline.h>
#include <engine/executable/runtime.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/internal/render/graphic_api/glad_graphic_api.h>

#include <iostream>

using Runtime = engine::Runtime;
using EditorRuntimePipeline = engine::EditorRuntimePipeline;
using Context = engine::IRuntimePipelineContext;

int main()
{
	MEMORY_GUARD;

	Runtime runtime;
	Context context = engine::RuntimePipelineContext<engine::GladGraphicAPI>{};
	runtime.setPipeline<EditorRuntimePipeline>();
	runtime.initialize(context);
	runtime.run();
}