#pragma once

#include <engine/editor/gizmo/gizmo.h>
#include <engine/internal/core/constants/editor_constants.h>

namespace engine
{
	template<GizmoType Type>
	struct GizmoElement
	{};

	struct GizmoElementTranslate
	{
		static constexpr Vector4 selectionColor = Vector4::zero();
		static constexpr Vector4 drawingColor = Vector4::zero();

		static constexpr std::vector<std::uint32_t> indices()
		{
			std::vector<std::uint32_t> indices =
			{
				0, 1
			};
			return indices;
		}
	};

	template<>
	struct GizmoElement<GizmoType::TranslateX> : public GizmoElementTranslate
	{
		static constexpr Vector4 selectionColor = constants::X_TRANSLATION_GIZMO_CODE;
		static constexpr Vector4 drawingColor = constants::X_GIZMO_DRAWING_COLOR;

		static constexpr std::vector<Vertex> vertices()
		{
			std::vector<Vertex> vertices =
			{
				Vertex(Vector3(0, 0, 0)),
				Vertex(Vector3(1, 0, 0)),
			};
			return vertices;
		}
	};

	template<>
	struct GizmoElement<GizmoType::TranslateY> : public GizmoElementTranslate
	{
		static constexpr Vector4 selectionColor = constants::Y_TRANSLATION_GIZMO_CODE;
		static constexpr Vector4 drawingColor = constants::Y_GIZMO_DRAWING_COLOR;

		static constexpr std::vector<Vertex> vertices()
		{
			std::vector<Vertex> vertices =
			{
				Vertex(Vector3(0, 0, 0)),
				Vertex(Vector3(0, 1, 0)),
			};
			return vertices;
		}
	};

	template<>
	struct GizmoElement<GizmoType::TranslateZ> : public GizmoElementTranslate
	{
		static constexpr Vector4 selectionColor = constants::Z_TRANSLATION_GIZMO_CODE;
		static constexpr Vector4 drawingColor = constants::Z_GIZMO_DRAWING_COLOR;

		static constexpr std::vector<Vertex> vertices()
		{
			std::vector<Vertex> vertices =
			{
				Vertex(Vector3(0, 0, 0)),
				Vertex(Vector3(0, 0, 1)),
			};
			return vertices;
		}
	};
} // namespace engine