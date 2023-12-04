#pragma once

#include <engine/executable/production_runtime_pipeline.h>

namespace engine
{
	class Window;
	class EditorRuntimePipeline final : public ProductionRuntimePipeline
	{
		// friends
	private:
		friend class Main;

		// members
	private:
		explicit EditorRuntimePipeline();

		virtual bool initialize() override;
		virtual void finalize() override;
		virtual void run() override;

		bool initializeEditor();
		bool createEditorWindow();

		const Window* m_editorWindow;
	};
} // namespace engine
