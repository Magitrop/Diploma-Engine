#pragma once

#include <queue>
#include <list>
#include <array>
#include <memory>

namespace engine
{
	// Should be changed if page capacity is supposed to be larger than 65536.
	using PageCapacityType = std::uint16_t;

	// A vector that does not shift elements when erasing or reallocating.
	// - Note that PageCapacity is NOT measured in bytes. It represents the maximum amount of elements per page.
	// - Set PageCapacity as big as necessary in order to improve performance.
	template<typename Type, PageCapacityType PageCapacity = 2048>
	class PersistentVector final
	{
	private:
		// A slot with a persistent address.
		struct Slot final
		{
			Slot();
			Slot(Type& value);
			Slot(Type&& value);

			Type& get();
			const Type& get() const;
			bool empty() const;

		private:
			Type m_value;
			bool m_empty;
		};

		using This = PersistentVector<Type>;
		using Page = std::array<Slot, PageCapacity>;
		using Pages = std::list<std::shared_ptr<Page>>;

	public:
		constexpr explicit PersistentVector();

		Type& push(Type& value);
		Type& push(Type&& value);
		void remove(Pages::iterator page, PageCapacityType index);

		[[nodiscard]] Pages::const_iterator begin() const;
		[[nodiscard]] Pages::const_iterator end() const;

		[[nodiscard]] std::size_t size() const;

	private:
		Type& pushInternal(Slot&& slot);
		void createPage();

		struct EmptyIndex
		{
			std::shared_ptr<Page> page;
			PageCapacityType index;
		};

		Pages m_pages;
		std::queue<EmptyIndex> m_emptyIndices;
		PageCapacityType m_currentIndex = 0;
		std::size_t m_count = 0;
	};

	template<typename Type, PageCapacityType PageCapacity>
	constexpr PersistentVector<Type, PageCapacity>::PersistentVector()
	{
		createPage();
	}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::push(Type& value)
	{
		return pushInternal(std::move(Slot(value)));
	}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::push(Type&& value)
	{
		return pushInternal(std::move(Slot(std::move(value))));
	}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::pushInternal(Slot&& slot)
	{
		++m_count;
		if (m_emptyIndices.empty())
		{
			if (m_currentIndex >= PageCapacity)
			{
				createPage();
				m_currentIndex = 0;
			}

			auto& result = (*m_pages.back())[m_currentIndex];
			result = std::move(slot);

			++m_currentIndex;

			return result.get();
		}
		else
		{
			EmptyIndex empty = m_emptyIndices.front();
			m_emptyIndices.pop();

			auto& result = (*empty.page)[empty.index];
			result = std::move(slot);

			return result.get();
		}
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::remove(Pages::iterator page, PageCapacityType index)
	{
		auto& slot = (**page)[index];
		if (slot.empty())
			return;

		m_emptyIndices.push({ *page, index });
		slot = std::move(Slot());

		--m_count;
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Pages::const_iterator PersistentVector<Type, PageCapacity>::begin() const
	{
		return m_pages.begin();
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Pages::const_iterator PersistentVector<Type, PageCapacity>::end() const
	{
		return m_pages.end();
	}

	template<typename Type, PageCapacityType PageCapacity>
	std::size_t PersistentVector<Type, PageCapacity>::size() const
	{
		return m_count;
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::createPage()
	{
		&m_pages.emplace_back(std::make_shared<Page>());
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Slot::Slot()
		: m_empty(true)
	{}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Slot::Slot(Type& value)
		: m_value(value)
		, m_empty(false)
	{}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Slot::Slot(Type&& value)
		: m_value(std::move(value))
		, m_empty(false)
	{}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::Slot::get()
	{
		return m_value;
	}

	template<typename Type, PageCapacityType PageCapacity>
	const Type& PersistentVector<Type, PageCapacity>::Slot::get() const
	{
		return m_value;
	}

	template<typename Type, PageCapacityType PageCapacity>
	bool PersistentVector<Type, PageCapacity>::Slot::empty() const
	{
		return m_empty;
	}
} // namespace engine