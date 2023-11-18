#pragma once

#include <queue>
#include <list>
#include <array>

namespace engine
{
	// A node with a persistent address.
	template<typename Type>
	struct PersistentNode final
	{
		PersistentNode();
		PersistentNode(Type& value);
		PersistentNode(Type&& value);

		Type& get();
		const Type& get() const;
		bool empty() const;

	private:
		Type m_value;
		bool m_empty;
	};

	// A vector that does not shift elements when erasing.
	template<typename Type, std::uint16_t PageCapacity = 2048>
	class PersistentVector final
	{
	private:
		using This = PersistentVector<Type>;
		using Page = std::array<PersistentNode<Type>, PageCapacity>;

	public:
		explicit PersistentVector();

		Type& push(Type& value);
		Type& push(Type&& value);
		void remove(std::size_t at);

		std::size_t size() const;

		Type& operator [] (std::size_t at);
		const Type& operator [] (std::size_t at) const;

	private:
		Type& pushInternal(PersistentNode<Type>&& node);
		void createPage();

		struct EmptyIndex
		{
			Page* page;
			std::size_t index;
		};

		std::queue<EmptyIndex> m_emptyIndices;
		std::list<Page> m_pages;
		std::uint16_t m_currentIndex = 0;
		std::size_t m_count = 0;
	};

	template<typename Type, std::uint16_t PageCapacity>
	PersistentVector<Type, PageCapacity>::PersistentVector()
	{
		createPage();
	}

	template<typename Type, std::uint16_t PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::push(Type& value)
	{
		return pushInternal(std::move(PersistentNode<Type>(value)));
	}

	template<typename Type, std::uint16_t PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::push(Type&& value)
	{
		return pushInternal(std::move(PersistentNode<Type>(std::move(value))));
	}

	template<typename Type, std::uint16_t PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::pushInternal(PersistentNode<Type>&& node)
	{
		++m_count;
		if (m_emptyIndices.empty())
		{
			if (m_currentIndex >= PageCapacity)
			{
				createPage();
				m_currentIndex = 0;
			}

			auto& result = m_pages.back()[m_currentIndex];
			result = std::move(node);

			++m_currentIndex;

			return result.get();
		}
		else
		{
			EmptyIndex empty = m_emptyIndices.front();
			m_emptyIndices.pop();

			auto& result = (*empty.page)[empty.index];
			result = std::move(node);

			return result.get();
		}
	}

	template<typename Type, std::uint16_t PageCapacity>
	void PersistentVector<Type, PageCapacity>::remove(std::size_t at)
	{
		// TODO: should throw?
		if (at >= m_pages.size() * PageCapacity)
			return;

		std::size_t page = at / PageCapacity;
		std::uint16_t index = at % PageCapacity;

		auto it = m_pages.begin();
		std::advance(it, page);

		auto& node = (*it)[index];
		if (node.empty())
			return;

		m_emptyIndices.push({ &*it, index });
		node = std::move(PersistentNode<Type>());

		--m_count;
	}

	template<typename Type, std::uint16_t PageCapacity>
	std::size_t PersistentVector<Type, PageCapacity>::size() const
	{
		return m_count;
	}

	template<typename Type, std::uint16_t PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::operator [] (std::size_t at)
	{
		std::size_t page = at / PageCapacity;
		std::uint16_t index = at % PageCapacity;

		auto it = m_pages.begin();
		std::advance(it, page);

		return (*it)[index].get();
	}

	template<typename Type, std::uint16_t PageCapacity>
	const Type& PersistentVector<Type, PageCapacity>::operator[](std::size_t at) const
	{
		std::size_t page = at / PageCapacity;
		std::uint16_t index = at % PageCapacity;

		auto it = m_pages.begin();
		std::advance(it, page);

		return (*it)[index].get();
	}

	template<typename Type, std::uint16_t PageCapacity>
	void PersistentVector<Type, PageCapacity>::createPage()
	{
		&m_pages.emplace_back(std::move(Page()));
	}

	template<typename Type>
	PersistentNode<Type>::PersistentNode()
		: m_empty(true)
	{}

	template<typename Type>
	PersistentNode<Type>::PersistentNode(Type& value)
		: m_value(value)
		, m_empty(false)
	{}

	template<typename Type>
	PersistentNode<Type>::PersistentNode(Type&& value)
		: m_value(std::move(value))
		, m_empty(false)
	{}

	template<typename Type>
	Type& PersistentNode<Type>::get()
	{
		return m_value;
	}

	template<typename Type>
	const Type& PersistentNode<Type>::get() const
	{
		return m_value;
	}

	template<typename Type>
	bool PersistentNode<Type>::empty() const
	{
		return m_empty;
	}
} // namespace engine