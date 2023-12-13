#pragma once

#include <engine/executable/production_runtime_pipeline.h>
#include <engine/render/window/window.h>

namespace engine
{
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

		WindowID m_editorWindow;
	};
} // namespace engine
