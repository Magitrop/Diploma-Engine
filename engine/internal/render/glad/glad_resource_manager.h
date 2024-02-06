#pragma once

#include <engine/core/resources/resource_registrar.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/internal/render/glad/glad_material.h>
#include <engine/internal/render/glad/glad_mesh.h>
#include <engine/internal/render/glad/glad_shader.h>

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

		GladMaterialImpl* registerMaterial(std::string materialName,
										   ShaderID shader,
										   std::int8_t renderQueue);

		/*virtual */ShaderID findShader(std::string name) const/* override*/;
		virtual MaterialID findMaterial(std::string name) const override;
		//virtual MeshID findMesh(std::string name) const override;

		/*virtual */const GladShader* getShaderByID(ShaderID id) const/* override*/;
		virtual std::shared_ptr<MaterialAccessor> getMaterialByID(MaterialID id) override;
		virtual std::shared_ptr<MeshAccessor> getMeshByID(MeshID id) override;

		GladMaterialImpl* getMaterial(MaterialID id);
		GladMeshImpl* getMeshImpl(MeshID id);

		const GladMaterialImpl* getDefaultMaterial();

		virtual MeshID createMesh() override;

		void setMeshRenderer(std::shared_ptr<GladMeshRenderer> meshRenderer);

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
			GladMaterialImpl material;
		};
		PersistentVector<MaterialEntry, constants::MATERIALS_PER_PAGE> m_materials;
		GladMaterialImpl* m_defaultMaterial;

		struct MeshEntry
		{
			GladMeshImpl impl;
		};
		PersistentVector<MeshEntry, constants::MESHES_PER_PAGE> m_meshes;

		std::shared_ptr<GladMeshRenderer> m_meshRenderer;
	};
} // namespace engine
