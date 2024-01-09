#include "glad_resource_manager.h"

#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component.h>

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

	std::shared_ptr<MaterialAccessor> GladResourceManager::getMaterialByID(MaterialID id)
	{
		auto it = m_materials.at(id);
		if (it != m_materials.end())
			return std::make_shared<GladMaterialAccessor>(&it->get().material);
		return std::make_shared<GladMaterialAccessor>(nullptr);
	}

	std::shared_ptr<MeshAccessor> GladResourceManager::getMeshByID(MeshID id)
	{
		auto it = m_meshes.at(id);
		if (it != m_meshes.end())
			return std::make_shared<GladMeshAccessor>(&it->get().impl);
		return std::make_shared<GladMeshAccessor>(nullptr);
	}

	GladMaterialImpl* GladResourceManager::getMaterial(MaterialID id)
	{
		auto it = m_materials.at(id);
		if (it != m_materials.end())
			return &it->get().material;
		return nullptr;
	}

	GladMeshImpl* GladResourceManager::getMeshImpl(MeshID id)
	{
		auto it = m_meshes.at(id);
		if (it != m_meshes.end())
			return &it->get().impl;
		return nullptr;
	}

	const GladMaterialImpl* GladResourceManager::getDefaultMaterial()
	{
		return m_defaultMaterial;
	}

	MeshID GladResourceManager::createMesh()
	{ 
		MEMORY_GUARD;

		GladMeshImpl mesh = GladMeshImpl(MeshID(m_meshes.getNextEmptyIndex()));
		mesh.connectOnMeshChanged(
			[this](MeshID id)
			{
				m_meshRenderer->updateMeshesWithID(id);
			}
		);

		return MeshID(m_meshes.push({ std::move(mesh) }).getIndex());
	}

	void GladResourceManager::setMeshRenderer(std::shared_ptr<GladMeshRenderer> meshRenderer)
	{
		m_meshRenderer = meshRenderer;
	}

	void GladResourceManager::registerBuiltinShaders()
	{
		MEMORY_GUARD;

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
		MEMORY_GUARD;

		for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
		{
			auto& [name, shader] = it->get();
			if (name == "default")
				m_defaultMaterial = registerMaterial("default", ShaderID(it.getIndex()));
		}
	}

	GladShader* GladResourceManager::registerShader(std::string shaderName,
													const GLchar* vertexCode, 
													const GLchar* fragmentCode)
	{
		MEMORY_GUARD;

		DEBUG_LOG("Registering shader \"{}\"...", shaderName);

		GladShader shader;
		if (shader.setupShader(shaderName, vertexCode, fragmentCode))
		{
			return &m_shaders.push({ shaderName, shader })->get().shader;
		}
		return nullptr;
	}

	GladMaterialImpl* GladResourceManager::registerMaterial(std::string materialName, ShaderID shader)
	{
		MEMORY_GUARD;

		DEBUG_LOG("Registering material \"{}\"...", materialName);

		GladShader* shaderPtr = &m_shaders.at(shader)->get().shader;
		return &m_materials.push({ materialName, GladMaterialImpl(shaderPtr) })->get().material;
	}
} // namespace engine