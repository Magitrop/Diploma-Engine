#pragma once

namespace engine
{
	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable(NonCopyable&) = delete;
		NonCopyable& operator = (NonCopyable&) = delete;
	};
} // namespace engine