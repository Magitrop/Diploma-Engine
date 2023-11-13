#include "runtime.h"

#include <engine/internal/render/window/window_manager.h>
#include <engine/executable/editor_runtime_pipeline.h>

using Runtime = engine::executable::Runtime;
using IRuntimePipeline = engine::executable::IRuntimePipeline;

Runtime::Runtime()
	: m_runtimePipeline(nullptr)
{}

bool engine::executable::Runtime::isRunning() const
{
	return m_runtimePipeline->isRunning();
}

void Runtime::initialize()
{
	m_runtimePipeline->initialize();
}

void Runtime::run()
{
	m_runtimePipeline->run();
}
