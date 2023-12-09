#pragma once

namespace engine
{
	class IRenderPipeline
	{
	protected:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;
	};

	/*
	
	Default Render Pipeline:

	1. Culling (frustum culling, occlusion culling, enabled/disabled objects etc.)
	2. Rendering
	3. Post-processing (optional)
	
	*/
} // namespace engine
