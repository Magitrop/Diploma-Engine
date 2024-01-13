#pragma once

#include <memory>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/editor/viewport/editor_camera.h>

namespace engine
{
	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = 0;

		virtual std::uint32_t width() const = 0;
		virtual std::uint32_t height() const = 0;
	};

	class IRenderPipeline
	{
		// friends
	private:
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;

		// members
	protected:
#if IS_EDITOR
		virtual bool initialize(std::shared_ptr<EditorViewports> viewports) = 0;
		virtual void renderEditorViewports() = 0;
		virtual void renderEditorSimulation() = 0;
		
		EditorCamera createEditorCamera(std::uint32_t initialViewportWidth,
										std::uint32_t initialViewportHeight);
		virtual std::shared_ptr<IFramebuffer> createFramebuffer(std::uint32_t width,
																std::uint32_t height) = 0;

		std::shared_ptr<EditorViewports> m_editorViewports;
#else
		virtual bool initialize(std::shared_ptr<CameraComponent> cameraComponent) = 0;
		virtual void renderFrame() = 0;

		std::shared_ptr<CameraComponent> m_cameraComponent;
#endif // #if IS_EDITOR

		virtual void finalize() = 0;
	};
} // namespace engine
