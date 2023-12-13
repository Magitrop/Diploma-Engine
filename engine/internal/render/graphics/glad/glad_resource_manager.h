#pragma once

#include <engine/core/resources/resource_registrar.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/internal/render/graphics/glad/glad_material.h>
#include <engine/internal/render/graphics/glad/glad_shader.h>

namespace engine
{
	class GladResourceManager final : public ResourceRegistrar
	{
		// friends
	private:
		friend class GladRenderPipeline;

		// members
	public:
		virtual void registerBuiltinShaders() override;
		virtual void registerBuiltinMaterials() override;

		GladShader* registerShader(std::string shaderName,
								   const GLchar* vertexCode,
								   const GLchar* fragmentCode);

		GladMaterial* registerMaterial(std::string materialName,
									   ShaderID shader);

		/*virtual */ShaderID findShader(std::string name) const/* override*/;
		virtual MaterialID findMaterial(std::string name) const override;

		/*virtual */const GladShader* getShaderByID(ShaderID id) const/* override*/;
		virtual const GladMaterial* getMaterialByID(MaterialID id) const override;

	private:
		struct ShaderEntry
		{
			std::string name;
			GladShader shader;
		};
		PersistentVector<ShaderEntry, constants::SHADERS_PER_PAGE> m_shaders;

		struct MaterialEntry
		{
			std::string name;
			GladMaterial material;
		};
		PersistentVector<MaterialEntry, constants::MATERIALS_PER_PAGE> m_materials;
	};
} // namespace engine
