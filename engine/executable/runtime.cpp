#include "runtime.h"

#include <engine/internal/render/window/window_manager.h>
#include <engine/executable/editor_runtime_pipeline.h>

using Runtime = engine::executable::Runtime;

Runtime::Runtime()
	: m_runtimePipeline(nullptr)
{}

void Runtime::initialize()
{
	m_runtimePipeline->initialize();
}

std::shared_ptr<const engine::executable::RuntimePipeline> Runtime::getPipeline() const
{
	return m_runtimePipeline;
}
