#pragma once

#include <engine/core/components/builtin_component.h>
#include <engine/core/resources/material.h>

namespace engine
{
	class Renderer : public BuiltinComponent 
	{
	public:
		virtual void setMaterial(ComponentID componentID, MaterialID materialID) = 0;
	};
} // namespace engine