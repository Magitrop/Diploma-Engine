#include "glad_resource_manager.h"

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/render/graphics/glad/glad_shader.h>

namespace engine
{
	ShaderID GladResourceManager::findShader(std::string name) const
	{
		for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
		{
			auto& entry = it->get();
			if (entry.name == name)
				return ShaderID(it.getIndex());
		}
		return ShaderID();
	}

	MaterialID GladResourceManager::findMaterial(std::string name) const
	{
		for (auto it = m_materials.begin(); it != m_materials.end(); ++it)
		{
			auto& entry = it->get();
			if (entry.name == name)
				return MaterialID(it.getIndex());
		}
		return MaterialID();
	}

	const GladShader* GladResourceManager::getShaderByID(ShaderID id) const
	{
		auto it = m_shaders.at(id);
		if (it != m_shaders.end())
			return &it->get().shader;
		return nullptr;
	}

	const GladMaterial* GladResourceManager::getMaterialByID(MaterialID id) const
	{
		auto it = m_materials.at(id);
		if (it != m_materials.end())
			return &it->get().material;
		return nullptr;
	}

	void GladResourceManager::registerBuiltinShaders()
	{
		registerShader("default", 
R"(
#version 460
layout(location = 0) in vec3 vertexPos;
void main()
{
	gl_Position.xyz = vertexPos;
	gl_Position.w = 1.0;
}
)",
R"(
#version 460
out vec3 color;
void main() 
{ 
	color = vec3(0, 0, 1); 
}
)");
	}

	void GladResourceManager::registerBuiltinMaterials()
	{
		for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
		{
			auto& [name, shader] = it->get();
			if (name == "default")
				registerMaterial("default", ShaderID(it.getIndex()));
		}
	}

	GladShader* GladResourceManager::registerShader(std::string shaderName,
													const GLchar* vertexCode, 
													const GLchar* fragmentCode)
	{
		DEBUG_LOG("Registering shader \"{}\"...", shaderName);
		GladShader shader;
		if (shader.setupShader(shaderName, vertexCode, fragmentCode))
		{
			return &m_shaders.push({ shaderName, shader })->get().shader;
		}
		return nullptr;
	}

	GladMaterial* GladResourceManager::registerMaterial(std::string materialName, ShaderID shader)
	{
		DEBUG_LOG("Registering material \"{}\"...", materialName);
		return &m_materials.push({ materialName, GladMaterial(shader) })->get().material;
	}
} // namespace engine