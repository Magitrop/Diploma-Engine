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
		struct Page
		{
			std::array<Slot, PageCapacity> slots;
			std::size_t pageIndex = static_cast<std::size_t>(-1);
		};
		using Pages = std::list<std::shared_ptr<Page>>;

		class Iterator final
		{
		private:
			friend class PersistentVector;

			Iterator(Pages::const_iterator page,
				PageCapacityType index,
				Pages::const_iterator end)
				: m_page(page)
				, m_index(index)
				, m_end(end)
			{}
		public:
			Type& operator * ();
			Type* operator -> ();
			const Type& operator * () const;
			const Type* operator -> () const;
			bool operator != (const Iterator& other) const;
			bool operator == (const Iterator& other) const;
			Iterator& operator ++();
			[[nodiscard]] std::size_t getIndex() const; // Returns an absolute index of the value. Does not take empty slots into account.

		private:
			Pages::const_iterator page() const;
			PageCapacityType index() const;

			Pages::const_iterator m_end;
			Pages::const_iterator m_page;
			PageCapacityType m_index;
		};

	public:
		constexpr explicit PersistentVector();

		// Returns an index at which the value was emplaced.
		Type& push(Type& value, std::size_t* index = nullptr);
		// Returns an index at which the value was emplaced.
		Type& push(Type&& value, std::size_t* index = nullptr);
		void remove(const Iterator at);
		void clear();
		// Clears the vector and invalidates all its iterators, pointers and references.
		// Use only if you sure what you are doing. Otherwise, consider using clear().
		void forceClear();

		[[nodiscard]] const Iterator begin() const;
		[[nodiscard]] const Iterator end() const;
		[[nodiscard]] Iterator begin();
		[[nodiscard]] Iterator end();
		[[nodiscard]] Iterator find(const Type& value);

		[[nodiscard]] Iterator at(std::size_t index);
		[[nodiscard]] const Iterator at(std::size_t index) const;

		[[nodiscard]] std::size_t size() const;

	private:
		Type& pushInternal(Slot&& slot, std::size_t* index = nullptr);
		void createPage();

		struct EmptyIndex
		{
			Pages::const_iterator page;
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
	Type& PersistentVector<Type, PageCapacity>::push(Type& value, std::size_t* index /* = nullptr */)
	{
		return pushInternal(std::move(Slot(value)), index);
	}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::push(Type&& value, std::size_t* index /* = nullptr */)
	{
		return pushInternal(std::move(Slot(std::move(value))), index);
	}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::pushInternal(Slot&& slot, std::size_t* index /* = nullptr */)
	{
		++m_count;
		if (m_emptyIndices.empty())
		{
			if (m_currentIndex >= PageCapacity)
			{
				createPage();
				m_currentIndex = 0;
			}

			auto& result = m_pages.back()->slots[m_currentIndex];
			result = std::move(slot);

			++m_currentIndex;

			if (index)
				*index = (m_pages.size() - 1) * PageCapacity + (m_currentIndex - 1);
			return result.get();
		}
		else
		{
			EmptyIndex empty = m_emptyIndices.front();
			m_emptyIndices.pop();

			auto& result = (*empty.page)->slots[empty.index];
			result = std::move(slot);

			if (index)
				*index = ((*empty.page)->pageIndex * PageCapacity) + empty.index;
			return result.get();
		}
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::remove(const Iterator at)
	{
		if (at == end())
		{
			return;
		}

		auto& slot = (*at.page())->slots[at.index()];
		if (slot.empty())
			return;

		m_emptyIndices.push({ (at.page()), at.index() });
		slot = std::move(Slot());

		--m_count;
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::clear()
	{
		for (auto it = begin(); it != end(); ++it)
		{
			auto& slot = (*it.page())->slots[it.index()];
			if (slot.empty())
				continue;

			m_emptyIndices.push({ (it.page()), it.index() });
			slot = std::move(Slot());
		}

		m_currentIndex = 0;
		m_count = 0;
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::forceClear()
	{
		m_pages.clear();
		std::queue<EmptyIndex>().swap(m_emptyIndices); // clear empty indices
		m_currentIndex = 0;
		m_count = 0;
	}

	template<typename Type, PageCapacityType PageCapacity>
	const typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::begin() const
	{
		return Iterator(m_pages.begin(), 0, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	const typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::end() const
	{
		return Iterator(m_pages.end(), 0, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::begin()
	{
		return Iterator(m_pages.begin(), 0, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::end()
	{
		return Iterator(m_pages.end(), 0, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::find(const Type& value)
	{
		for (auto it = begin(); it != end(); ++it)
		{
			auto v = *it;
			if (v == value)
			{
				return it;
			}
		}
		return Iterator(m_pages.end(), 0, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::at(std::size_t index)
	{
		std::size_t page = index / PageCapacity;
		std::size_t idx = index % PageCapacity;

		if (page >= m_pages.size())
		{
			return end();
		}

		auto it = m_pages.begin();
		std::advance(it, page);

		return Iterator(it, idx, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	const typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::at(std::size_t index) const
	{
		std::size_t page = index / PageCapacity;
		std::size_t idx = index % PageCapacity;

		if (page >= m_pages.size())
		{
			return end();
		}

		auto it = m_pages.begin();
		std::advance(it, page);

		return Iterator(it, idx, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	std::size_t PersistentVector<Type, PageCapacity>::size() const
	{
		return m_count;
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::createPage()
	{
		auto page = m_pages.emplace_back(std::make_shared<Page>());
		page->pageIndex = m_pages.size() - 1;
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

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::Iterator::operator * ()
	{
		return (*m_page)->slots[m_index].get();
	}

	template<typename Type, PageCapacityType PageCapacity>
	const Type& PersistentVector<Type, PageCapacity>::Iterator::operator * () const
	{
		return (*m_page)->slots[m_index].get();
	}

	template<typename Type, PageCapacityType PageCapacity>
	Type* PersistentVector<Type, PageCapacity>::Iterator::operator -> ()
	{
		return &(*m_page)->slots[m_index].get();
	}

	template<typename Type, PageCapacityType PageCapacity>
	const Type* PersistentVector<Type, PageCapacity>::Iterator::operator -> () const
	{
		return &(*m_page)->slots[m_index].get();
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Iterator& PersistentVector<Type, PageCapacity>::Iterator::operator ++()
	{
		if (m_page == m_end)
			return *this;

		do
		{
			++m_index;
			if (m_index >= PageCapacity)
			{
				m_index = 0;
				std::advance(m_page, 1);
			}

			if (m_page == m_end)
				break;
		} while ((*m_page)->slots[m_index].empty());
		return *this;
	}

	template<typename Type, PageCapacityType PageCapacity>
	bool PersistentVector<Type, PageCapacity>::Iterator::operator != (const Iterator& other) const
	{
		return m_page != other.m_page || m_index != other.m_index;
	}

	template<typename Type, PageCapacityType PageCapacity>
	bool PersistentVector<Type, PageCapacity>::Iterator::operator == (const Iterator& other) const
	{
		return m_page == other.m_page && m_index == other.m_index;
	}

	template<typename Type, PageCapacityType PageCapacity>
	std::size_t PersistentVector<Type, PageCapacity>::Iterator::getIndex() const
	{
		if (m_page == m_end)
			return static_cast<std::size_t>(-1);
		return (*m_page)->pageIndex * PageCapacity + m_index;
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Pages::const_iterator PersistentVector<Type, PageCapacity>::Iterator::page() const
	{
		return m_page;
	}

	template<typename Type, PageCapacityType PageCapacity>
	PageCapacityType PersistentVector<Type, PageCapacity>::Iterator::index() const
	{
		return m_index;
	}
} // namespace engine