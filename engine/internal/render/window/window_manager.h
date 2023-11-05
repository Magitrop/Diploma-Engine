#pragma once

namespace engine
{
	namespace internal
	{
		namespace render
		{
			class WindowManager final
			{
			public:
				explicit WindowManager() = default;
				WindowManager& operator = (WindowManager&) = delete;
			};
		} // namespace render
	} // namespace internal
} // namespace engine