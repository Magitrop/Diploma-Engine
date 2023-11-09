#include <engine/executable/editor_runtime_pipeline.h>
#include <engine/executable/runtime.h>

#include <iostream>

using Runtime = engine::executable::Runtime;
using EditorRuntimePipeline = engine::executable::EditorRuntimePipeline;

int main()
{
	Runtime runtime;
	runtime.setPipeline<EditorRuntimePipeline>();
	runtime.initialize();
}