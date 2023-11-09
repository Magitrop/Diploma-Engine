#pragma once

#include <memory>

namespace engine
{
	namespace internal
	{
		namespace helpers
		{
			template<typename Type>
			class Constructible final
			{
				friend Type;

				struct ConstructibleBase
				{
				protected:
					template<typename... Args>
					static Type construct(Args... args)
					{
						return Type(std::forward<decltype(args)>(args)...);
					}

					template<typename... Args>
					static Type* constructRaw(Args... args)
					{
						return new Type(std::forward<decltype(args)>(args)...);
					}

					template<typename... Args>
					static std::shared_ptr<Type> constructShared(Args... args)
					{
						return std::make_shared<Type>(construct(std::forward<decltype(args)>(args)...));
					}
				};

				template<typename... Friends>
				struct ConstructibleBy {};

				template<typename Friend>
				struct ConstructibleBy<Friend> : ConstructibleBase { friend Friend; };

				template<typename Friend, typename... Friends>
				struct ConstructibleBy<Friend, Friends...> : ConstructibleBy<Friend>, ConstructibleBy<Friends...> {};
			};
		} // namespace helpers
	} // namespace internal

	template<typename Type>
	using Constructible = engine::internal::helpers::Constructible<Type>;
} // namespace engine
