#pragma once

#include <engine/internal/helpers/helpers/construction_helper.h>

namespace engine
{
	namespace executable
	{
		class Runtime;
	} // namespace executable

	namespace editor
	{
		class EditorInitializer final : ConstructionHelper<EditorInitializer>
		{
			friend class ConstructionHelper;
			friend class engine::executable::Runtime;

		private:
			explicit EditorInitializer() = default;
		};
	} // namespace editor
} // namespace engine
