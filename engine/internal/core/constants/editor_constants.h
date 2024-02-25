#pragma once

#include <cstddef>

#include <engine/core/math/vector4.h>

namespace engine
{
	namespace constants
	{
		constexpr std::size_t VIEWPORTS_PER_PAGE = 8; // The amount of viewports per page in the Editor Viewports Manager.

		constexpr float SELECTION_CODE_STEP = 1.0f / 255.0f; // The minimum step needed to distinguish two entities on the selection frambuffer.
		constexpr Vector4 X_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, (1 << 0) * SELECTION_CODE_STEP); // The color code of the X axis of translation gizmo on the selection framebuffer.
		constexpr Vector4 Y_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, (1 << 1) * SELECTION_CODE_STEP); // The color code of the Y axis of translation gizmo on the selection framebuffer.
		constexpr Vector4 Z_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, (1 << 2) * SELECTION_CODE_STEP); // The color code of the Z axis of translation gizmo on the selection framebuffer.
		constexpr Vector4 XY_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 0) + (1 << 1)) * SELECTION_CODE_STEP); // The color code of the XY axes of translation gizmo on the selection framebuffer.
		constexpr Vector4 XZ_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 0) + (1 << 2)) * SELECTION_CODE_STEP); // The color code of the XZ axes of translation gizmo on the selection framebuffer.
		constexpr Vector4 YZ_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 1) + (1 << 2)) * SELECTION_CODE_STEP); // The color code of the YZ axes of translation gizmo on the selection framebuffer.
		constexpr Vector4 XYZ_TRANSLATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 3) - 1) * SELECTION_CODE_STEP); // The color code of common translation gizmo on the selection framebuffer.
		constexpr Vector4 X_ROTATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 3) + 0) * SELECTION_CODE_STEP); // The color code of the X axis of rotation gizmo on the selection framebuffer.
		constexpr Vector4 Y_ROTATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 3) + 1) * SELECTION_CODE_STEP); // The color code of the Y axis of rotation gizmo on the selection framebuffer.
		constexpr Vector4 Z_ROTATION_GIZMO_CODE = Vector4(0, 0, 0, ((1 << 3) + 2) * SELECTION_CODE_STEP); // The color code of the Z axis of rotation gizmo on the selection framebuffer.
		constexpr Vector4 X_SCALING_GIZMO_CODE = Vector4(0, 0, 0, (1 << 4) * SELECTION_CODE_STEP); // The color code of the X axis of rotation gizmo on the selection framebuffer.
		constexpr Vector4 Y_SCALING_GIZMO_CODE = Vector4(0, 0, 0, (1 << 5) * SELECTION_CODE_STEP); // The color code of the Y axis of rotation gizmo on the selection framebuffer.
		constexpr Vector4 Z_SCALING_GIZMO_CODE = Vector4(0, 0, 0, (1 << 6) * SELECTION_CODE_STEP); // The color code of the Z axis of rotation gizmo on the selection framebuffer.
		constexpr Vector4 X_GIZMO_DRAWING_COLOR = Vector4(0.8f, 0, 0, 1);
		constexpr Vector4 Y_GIZMO_DRAWING_COLOR = Vector4(0, 0.8f, 0, 1);
		constexpr Vector4 Z_GIZMO_DRAWING_COLOR = Vector4(0, 0, 0.8f, 1);
	}
}