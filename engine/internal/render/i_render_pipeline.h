#pragma once

#include <memory>

#include <engine/internal/core/constants/runtime_constants.h>

namespace engine
{
	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = 0;

		virtual std::uint32_t width() const = 0;
		virtual std::uint32_t height() const = 0;
		virtual float aspectRatio() const = 0;
	};

#if IS_EDITOR
	class EditorViewportsManager;
	class EditorDrawerContext;
	class EditorDrawer;
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
#endif // #if IS_EDITOR

		// members
	protected:
#if IS_EDITOR
		virtual bool initialize(std::shared_ptr<EditorViewportsManager> viewports,
								std::shared_ptr<EditorDrawer> editorDrawer) = 0;

		virtual void renderEditorViewports() = 0;
		virtual void renderEditorSimulation() = 0;
		
		virtual std::shared_ptr<IFramebuffer> createFramebuffer(std::uint32_t width,
																std::uint32_t height) = 0;
#else
		virtual bool initialize(std::shared_ptr<CameraComponent> cameraComponent) = 0;
		virtual void renderFrame() = 0;

		std::shared_ptr<CameraComponent> m_cameraComponent;
#endif // #if IS_EDITOR

		virtual void finalize() = 0;
	};
} // namespace engine
