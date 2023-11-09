#include "runtime_accessor.h"

#include <engine/executable/runtime.h>
#include <engine/executable/runtime_pipeline.h>

using RuntimeAccessor = engine::executable::RuntimeAccessor;

const engine::executable::Runtime* RuntimeAccessor::runtime()
{
    return m_runtime;
}

const engine::executable::IPipelineStorage* engine::executable::RuntimeAccessor::storage()
{
    return m_runtime->getPipeline()->storage();
}
