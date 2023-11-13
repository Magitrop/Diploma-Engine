#pragma once

#include <engine/executable/production_runtime_pipeline.h>

#include <memory>

namespace engine
{
	namespace executable
	{
		class EditorRuntimePipeline final : public ProductionRuntimePipeline
		{
			// friends
		private:
			friend class Runtime;

			// members
		private:
			explicit EditorRuntimePipeline();

			virtual void initialize() override;
			virtual void run() override;

			void initializeEditor();

			const Window* m_editorWindow;
		};
	} // namespace executable
} // namespace engine
