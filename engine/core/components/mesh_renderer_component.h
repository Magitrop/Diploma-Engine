#pragma once

#include <engine/core/components/renderer_component.h>
#include <engine/core/resources/mesh.h>

namespace engine
{
	class MeshRenderer : public Renderer
	{
	public:
		virtual void setMesh(ComponentID componentID, MeshID meshID) = 0;
	};
	DEFINE_COMPONENT(MeshRenderer)
} // namespace engine
