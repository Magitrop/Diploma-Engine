#pragma once

#include <vector>
#include <stack>

namespace engine
{
	namespace internal
	{
		namespace helpers
		{
			// A node with a persistent address.
			template<typename Type>
			struct PersistentNode final
			{
				PersistentNode();
				PersistentNode(Type&& value);
			private:
				Type m_value;
			};

			// A vector that does not shift elements when erasing.
			template<typename Type>
			class PersistentVector final : private std::vector<PersistentNode<Type>>
			{
			private:
				using This = PersistentVector<Type>;
				using Base = std::vector<PersistentNode<Type>>;
			public:
				void push(Type& value);
				void push(Type&& value);
				void erase(std::size_t at);
				std::size_t count();

			private:
				std::stack<std::size_t> m_emptyIndices;
			};

			template<typename Type>
			void PersistentVector<Type>::push(Type& value)
			{
				auto index = This::end();
				if (!m_emptyIndices.empty())
					index = This::begin() + m_emptyIndices.top();

				Base::emplace(index, value);
			}

			template<typename Type>
			void PersistentVector<Type>::push(Type&& value)
			{
				if (m_emptyIndices.empty())
				{
					Base::push_back(std::move(value));
				}
				else
				{
					auto index = This::begin() + m_emptyIndices.top();
					*index = std::move(value);
				}
			}

			template<typename Type>
			void PersistentVector<Type>::erase(std::size_t at)
			{
				// TODO: should throw?
				if (at >= Base::size() || at < 0)
					return;

				auto index = This::begin() + at;
				m_emptyIndices.push(std::distance(This::begin(), index));
				*index = std::move(PersistentNode<Type>());
			}

			template<typename Type>
			std::size_t PersistentVector<Type>::count()
			{
				// empty indices don't count
				return Base::size() - m_emptyIndices.size();
			}

			template<typename Type>
			PersistentNode<Type>::PersistentNode()
			{}

			template<typename Type>
			PersistentNode<Type>::PersistentNode(Type&& value)
				: m_value(std::move(value))
			{}
		} // namespace helpers
	} // namespace internal

	template<typename Type>
	using PersistentVector = engine::internal::helpers::PersistentVector<Type>;
} // namespace engine