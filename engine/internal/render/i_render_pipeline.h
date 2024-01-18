#pragma once

#include <memory>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/editor/viewport/editor_camera.h>
#include <engine/editor/gui/editor_viewport_window.h>

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

	class EditorViewports;
	class InputSystem;
	class IRenderPipeline
	{
		// friends
	private:
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;

		// members
	protected:
#if IS_EDITOR
		virtual bool initialize(std::shared_ptr<EditorViewports> viewports,
								std::shared_ptr<InputSystem> inputSystem) = 0;
		virtual void renderEditorViewports() = 0;
		virtual void renderEditorSimulation() = 0;
		
		EditorViewportWindow createEditorViewport(std::uint32_t initialViewportWidth,
												  std::uint32_t initialViewportHeight);
		virtual std::shared_ptr<IFramebuffer> createFramebuffer(std::uint32_t width,
																std::uint32_t height) = 0;

		std::shared_ptr<EditorViewports> m_editorViewports;
		std::shared_ptr<InputSystem> m_inputSystem;
#else
		virtual bool initialize(std::shared_ptr<CameraComponent> cameraComponent) = 0;
		virtual void renderFrame() = 0;

		std::shared_ptr<CameraComponent> m_cameraComponent;
#endif // #if IS_EDITOR

		virtual void finalize() = 0;
	};
} // namespace engine
