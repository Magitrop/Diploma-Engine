#pragma once

#include <engine/editor/editor.h>
#include <engine/executable/production_runtime_pipeline.h>
#include <engine/render/window/window.h>

namespace engine
{
	class EditorViewportsManager;
	class EditorRuntimePipeline final : public ProductionRuntimePipeline
	{
		// friends
	private:
		friend class Main;

		// members
	private:
		using Base = ProductionRuntimePipeline;

		explicit EditorRuntimePipeline();

		virtual bool initialize() override;
		virtual void finalize() override;
		virtual void run() override;

		[[nodiscard]] bool createEditorWindow();
		[[nodiscard]] bool initializeEditor();
		[[nodiscard]] bool initializeImGui();
		[[nodiscard]] virtual bool initializeRenderPipeline() override;

		void finalizeImGui();

		WindowID m_editorWindow;
		std::shared_ptr<Editor> m_editor;
	};
} // namespace engine
