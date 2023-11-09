#pragma once

#include <engine/executable/i_runtime_pipeline.h>
#include <engine/internal/helpers/helpers/construction_helper.h>

#include <memory>

int main();

namespace engine
{
	namespace executable
	{
		class RuntimePipeline : public IRuntimePipeline
		{
		public:
			virtual void initialize() = 0;
		};
	}
}
