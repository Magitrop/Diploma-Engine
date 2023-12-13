#pragma once

#include <memory>

#include <engine/internal/render/graphics/glad/glad_graphic_api.h>

#include <engine/core/components/render_component.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

#include <engine/dependencies/gl/glad/include/glad/glad.h>

namespace engine
{
	class GladRenderComponent final : public RenderComponent
	{
		// friends
	private:
		friend class GladRenderPipeline;
		friend class EditorRuntimePipeline;
		friend class ProductionRuntimePipeline;

		// members
	private:
		GladRenderComponent() = default;

	protected:
		virtual void attachComponent(ComponentID component) override;
		virtual void detachComponent(ComponentID component) override;

	public:
		virtual ~GladRenderComponent() override {};

		virtual void setMaterial(ComponentID componentID, MaterialID material) override;

	private:
		PersistentVector<GLuint, constants::VERY_FREQUENT_COMPONENT> m_VAO; // Vertex array object.
		PersistentVector<GLuint, constants::VERY_FREQUENT_COMPONENT> m_VBO; // Vertex buffer object.
		PersistentVector<MaterialID, constants::VERY_FREQUENT_COMPONENT> m_material;
	};
} // namespace engine
