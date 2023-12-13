#pragma once

namespace engine
{
	class IRenderPipeline
	{
		// friends
	private:
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;

		// members
	protected:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;

		virtual void renderFrame() = 0;
	};

	/*
	
	Default Render Pipeline:

	1. Culling (frustum culling, occlusion culling, enabled/disabled objects etc.)
	2. Rendering
	3. Post-processing (optional)
	
	*/
} // namespace engine
