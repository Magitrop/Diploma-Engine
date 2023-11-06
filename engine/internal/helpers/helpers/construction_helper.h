#pragma once

#include <memory>

namespace engine
{
	namespace internal
	{
		namespace helpers
		{
			template<typename Type>
			struct ConstructionHelper
			{
			protected:
				template<typename... Args>
				static Type construct(Args... args)
				{
					return Type(std::forward(args)...);
				}

				template<typename... Args>
				static std::shared_ptr<Type> constructShared(Args... args)
				{
					return std::make_shared<Type>(construct(std::forward(args)...));
				}
			};
		} // namespace helpers
	} // namespace internal

	template<typename Type>
	using ConstructionHelper = engine::internal::helpers::ConstructionHelper<Type>;
} // namespace engine
