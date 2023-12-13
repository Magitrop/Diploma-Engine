#pragma once

#include <engine/core/components/component_manager.h>
#include <engine/core/resources/material.h>

namespace engine
{
	class RenderComponent : public ComponentManager 
	{
	public:
		virtual void setMaterial(ComponentID componentID, MaterialID material) = 0;
	};
} // namespace engine