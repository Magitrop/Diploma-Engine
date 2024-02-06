#pragma once

#include <memory>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/render/i_framebuffer.h>

namespace engine
{
#if IS_EDITOR
	class EditorViewportsManager;
	class IEditorDrawerContext;
	class IEditorDrawer;
#endif // #if IS_EDITOR
	class CameraComponent;
	class InputSystem;
	class IRenderPipeline
	{
		// friends
	private:
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;
#if IS_EDITOR
		friend class Editor;
		friend class IEditorDrawer;
#endif // #if IS_EDITOR

	public:
		// members
#if IS_EDITOR
	protected:
		virtual bool initialize(std::shared_ptr<EditorViewportsManager> viewports,
								std::shared_ptr<IEditorDrawer> editorDrawer) = 0;

		virtual void renderEditorViewports() = 0;
		virtual void renderEditorSimulation() = 0;
		
	public:
		virtual std::shared_ptr<IFramebuffer> createFramebuffer(std::uint32_t width,
																std::uint32_t height) = 0;
#else
	protected:
		virtual bool initialize(std::shared_ptr<CameraComponent> cameraComponent) = 0;
		virtual void renderFrame() = 0;

		std::shared_ptr<CameraComponent> m_cameraComponent;
#endif // #if IS_EDITOR
	protected:
		virtual void finalize() = 0;
	};
} // namespace engine
