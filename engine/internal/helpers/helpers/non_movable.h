#pragma once

namespace engine
{
	namespace internal
	{
		namespace helpers
		{
			struct NonMovable
			{
				NonMovable() = default;
				NonMovable(NonMovable&&) = delete;
				NonMovable& operator = (NonMovable&&) = delete;
			};
		} // namespace helpers
	} // namespace internal

	using NonMovable = engine::internal::helpers::NonMovable;
} // namespace engine