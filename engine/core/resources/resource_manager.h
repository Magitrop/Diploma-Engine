#pragma once

#include <memory>
#include <string>

#include <engine/core/resources/material.h>
#include <engine/core/resources/shader.h>

namespace engine
{
	class ResourceManager
	{
	public:
		//virtual ShaderID findShader(std::string name) const = 0;
		virtual MaterialID findMaterial(std::string name) const = 0;

		//virtual const Shader* getShaderByID(ShaderID id) const = 0;
		virtual const Material* getMaterialByID(MaterialID id) const = 0;
	};
} // namespace engine