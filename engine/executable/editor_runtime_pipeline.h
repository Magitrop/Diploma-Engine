#pragma once

#include <engine/executable/runtime_pipeline.h>

#include <memory>

int main();

namespace engine
{
	namespace editor
	{
		class EditorInitializer;
	} // namespace editor

	namespace internal
	{
		namespace render
		{
			class WindowManager;
		} // namespace render
	} // namespace internal

	namespace executable
	{
		class EditorPipelineStorage : public IPipelineStorage
		{

		};

		class EditorRuntimePipeline final : public RuntimePipeline
		{
			friend Constructible<EditorRuntimePipeline>;
		private:
			using EditorInitializer = engine::editor::EditorInitializer;
			using WindowManager = engine::internal::render::WindowManager;

		private:
			explicit EditorRuntimePipeline() = default;

		public:
			using Constructor = Constructible<EditorRuntimePipeline>::ConstructibleBy<class Runtime>;
			void initialize();
			void runtime();

		private:
			virtual const EditorPipelineStorage* storage() const override;

			void initializeGL();
			void initializeEditor();
			void initializeWindowManager();

			EditorPipelineStorage m_storage;
			std::shared_ptr<EditorInitializer> m_editorInitializer;
			std::shared_ptr<WindowManager> m_windowManager;
		};
	} // namespace executable
} // namespace engine
