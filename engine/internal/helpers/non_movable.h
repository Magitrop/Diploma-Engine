#pragma once

namespace engine
{
	struct NonMovable
	{
		NonMovable() = default;
		NonMovable(NonMovable&&) = delete;
		NonMovable& operator = (NonMovable&&) = delete;
	};
} // namespace engine