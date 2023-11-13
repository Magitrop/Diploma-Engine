#pragma once

namespace engine
{
	namespace internal
	{
		namespace helpers
		{
			struct NonCopyable
			{
				NonCopyable() = default;
				NonCopyable(NonCopyable&) = delete;
				NonCopyable& operator = (NonCopyable&) = delete;
			};
		} // namespace helpers
	} // namespace internal

	using NonCopyable = engine::internal::helpers::NonCopyable;
} // namespace engine