#pragma once

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
		class Runtime final
		{
			friend int ::main();

			using EditorInitializer = engine::editor::EditorInitializer;
			using WindowManager = engine::internal::render::WindowManager;

		private:
			explicit Runtime() = default;
			Runtime(Runtime&) = delete;
			Runtime(Runtime&&) = delete;
			Runtime& operator = (Runtime&) = delete;
			Runtime& operator = (Runtime&&) = delete;

		public:
			void initialize();
			

		private:
			void initializeGL();
			void initializeEditor();
			void initializeWindowManager();

			std::shared_ptr<EditorInitializer> editorInitializer;
			std::shared_ptr<WindowManager> windowManager;
		};
	} // namespace executable
} // namespace engine
