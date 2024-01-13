#pragma once

#include <engine/editor/viewport/editor_camera.h>
#include <engine/executable/production_runtime_pipeline.h>
#include <engine/render/window/window.h>

namespace engine
{
	class EditorViewports;
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

		[[nodiscard]] bool initializeEditor();

		[[nodiscard]] bool initializeViewports();
		[[nodiscard]] bool createEditorWindow();
		[[nodiscard]] bool initializeImGui();
		[[nodiscard]] virtual bool initializeRenderPipeline() override;
		EditorCamera createEditorViewport(std::size_t initialViewportWidth,
										  std::size_t initialViewportHeight);

		void finalizeImGui();

		WindowID m_editorWindow;
		std::shared_ptr<EditorViewports> m_editorViewports;
	};
} // namespace engine
