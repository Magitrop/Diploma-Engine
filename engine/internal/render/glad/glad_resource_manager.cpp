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
layout (location = 0) in vec3 vertexPosition;

out VS_OUT 
{
	vec3 FragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vs_out.FragPos = vec3(model * vec4(vertexPosition, 1.0));
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}

)",
R"(
#version 460
out vec4 color;
in VS_OUT 
{
	vec3 FragPos;
} fs_in;

void main() 
{
	color = vec4(0, 0, 1, 1); 
}
)");

		registerShader("editor_grid",
			R"(
#version 460
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) out float near;
layout(location = 1) out float far;
layout(location = 2) out vec3 nearPoint;
layout(location = 3) out vec3 farPoint;
layout(location = 4) out mat4 fragView;
layout(location = 8) out mat4 fragProj;

// Grid position are in clipped space
vec3 gridPlane[6] = vec3[] 
(
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) 
{
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() 
{
    vec3 p = gridPlane[gl_VertexID].xyz;
    near = 0.01;
    far = 25;
    fragView = view;
    fragProj = projection;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz; // unprojecting on the far plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}
)",
R"(
#version 460

layout(location = 0) in float near; // 0.01
layout(location = 1) in float far; // 100
layout(location = 2) in vec3 nearPoint;
layout(location = 3) in vec3 farPoint;
layout(location = 4) in mat4 fragView;
layout(location = 8) in mat4 fragProj;

layout(location = 0) out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) 
{
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if (fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}
float computeDepth(vec3 pos) 
{
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}
float computeLinearDepth(vec3 pos) 
{
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}
void main() 
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    outColor = (grid(fragPos3D, 3, true) + grid(fragPos3D, 1, true)) * float(t > 0); // adding multiple resolution for the grid
    outColor.a *= fading;
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
			else if (name == "editor_grid")
				registerMaterial("editor_grid", ShaderID(it.getIndex()));
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