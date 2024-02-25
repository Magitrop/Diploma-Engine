#pragma once

#include <tuple>
#include <type_traits>

#include <engine/internal/helpers/assert.h>

namespace engine
{
	template<typename... Types>
	class Dependencies
	{
	public:
		struct DependencyContext final
		{
			DependencyContext(Types*... types)
				: m_types(std::tuple<Types*...>(types...))
			{}

			std::tuple<Types*...>& getTypes() { return m_types; }
			const std::tuple<Types*...>& getTypes() const { return m_types; }

		private:
			std::tuple<Types*...> m_types;
		};

	protected:
		explicit Dependencies(DependencyContext context)
			: m_dependencies(context)
		{}

		template<typename T>
		constexpr T* getDependency()
		{
			static_assert(std::disjunction<std::is_same<T, Types>...>::value, "Cannot get a dependency of that type.");
			T* result = std::get<T*>(m_dependencies.getTypes());
			DEBUG_ASSERT(result != nullptr);
			return result;
		}

		template<typename T>
		constexpr const T* getDependency() const
		{
			static_assert(std::disjunction<std::is_same<T, Types>...>::value, "Cannot get a dependency of that type.");
			T* result = std::get<T*>(m_dependencies.getTypes());
			DEBUG_ASSERT(result != nullptr);
			return result;
		}

	private:
		DependencyContext m_dependencies;
	};
} // namespace engine