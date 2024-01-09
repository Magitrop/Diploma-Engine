#pragma once

#include <memory>
#include <string>

#include <engine/core/resources/material.h>
#include <engine/core/resources/mesh.h>
#include <engine/core/resources/shader.h>

#include <engine/internal/helpers/non_copyable.h>

namespace engine
{
	class ResourceManager
	{
	public:
		//virtual ShaderID findShader(std::string name) const = 0;
		virtual MaterialID findMaterial(std::string name) const = 0;
		//virtual MeshID findMesh(std::string name) const = 0;

		//virtual const Shader* getShaderByID(ShaderID id) const = 0;
		virtual std::shared_ptr<MaterialAccessor> getMaterialByID(MaterialID id) = 0;
		virtual std::shared_ptr<MeshAccessor> getMeshByID(MeshID id) = 0;

		virtual MeshID createMesh() = 0;
	};
} // namespace engine