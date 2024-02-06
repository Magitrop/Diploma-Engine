#pragma once

#include <engine/dependencies/imgui/imgui.h>

namespace engine
{
	template<typename Type>
	struct ImGuiScopedStyleVar final
	{
		explicit ImGuiScopedStyleVar(ImGuiStyleVar_ var, Type value)
		{
			ImGui::PushStyleVar(var, value);
		}

		~ImGuiScopedStyleVar()
		{
			ImGui::PopStyleVar();
		}
	};
} // namespace engine