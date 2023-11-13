#include "i_runtime_pipeline.h"

using IRuntimePipeline = engine::executable::IRuntimePipeline;

bool IRuntimePipeline::isRunning()
{
    return m_isRunning;
}
