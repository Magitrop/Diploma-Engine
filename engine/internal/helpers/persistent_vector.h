#pragma once

#include <array>
#include <list>
#include <memory>
#include <queue>
#include <functional>

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/assert.h>

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
			Slot(Type value);

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

	public:
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
			Slot& operator * ();
			Slot* operator -> ();
			const Slot& operator * () const;
			const Slot* operator -> () const;
			bool operator != (const Iterator& other) const;
			bool operator == (const Iterator& other) const;
			Iterator& operator ++();
			Iterator& operator +(std::size_t diff);
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

		// Returns an iterator at which the value was emplaced.
		Iterator push(Type value);
		void remove(const Iterator at);
		void clear();
		// Clears the vector and invalidates all its iterators, pointers and references.
		// Use only if you are sure what you are doing. Otherwise, consider using clear().
		void forceClear();

		[[nodiscard]] const Iterator begin() const;
		[[nodiscard]] const Iterator end() const;
		[[nodiscard]] Iterator begin();
		[[nodiscard]] Iterator end();
		[[nodiscard]] Iterator find(const Type& value) const;

		using SearchPredicate = std::function<bool(const Type& value)>;
		[[nodiscard]] Iterator find(SearchPredicate predicate) const;

		[[nodiscard]] Iterator at(std::size_t index);
		[[nodiscard]] const Iterator at(std::size_t index) const;

		[[nodiscard]] std::size_t size() const;

		[[nodiscard]] std::size_t getNextEmptyIndex() const;

	private:
		Iterator pushInternal(Slot&& slot);
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
	PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::push(Type value)
	{
		return pushInternal(std::move(Slot(std::move(value))));
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::pushInternal(Slot&& slot)
	{
		++m_count;
		if (m_emptyIndices.empty())
		{
			if (m_currentIndex >= PageCapacity)
			{
				createPage();
				m_currentIndex = 0;
			}

			m_pages.back()->slots[m_currentIndex] = slot;
			++m_currentIndex;

			return Iterator(std::prev(m_pages.end()), m_currentIndex - 1, m_pages.end());
		}
		else
		{
			EmptyIndex empty = m_emptyIndices.front();
			m_emptyIndices.pop();

			(*empty.page)->slots[empty.index] = slot;

			return Iterator(empty.page, empty.index, m_pages.end());
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
		return m_count == 0 ? end() : Iterator(m_pages.begin(), 0, m_pages.end());
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
	typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::find(const Type& value) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (it->empty())
				continue;

			if (it->get() == value)
			{
				return it;
			}
		}
		return Iterator(m_pages.end(), 0, m_pages.end());
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Iterator PersistentVector<Type, PageCapacity>::find(SearchPredicate predicate) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (it->empty())
				continue;

			if (predicate(it->get()))
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
	std::size_t PersistentVector<Type, PageCapacity>::getNextEmptyIndex() const
	{
		if (m_emptyIndices.empty())
		{
			if (m_currentIndex >= PageCapacity)
				return (m_pages.back()->pageIndex + 1) * PageCapacity;
			else
				return m_pages.back()->pageIndex * PageCapacity + m_currentIndex;
		}
		else
		{
			EmptyIndex empty = m_emptyIndices.front();
			return (*empty.page)->pageIndex * PageCapacity + empty.index;
		}
	}

	template<typename Type, PageCapacityType PageCapacity>
	void PersistentVector<Type, PageCapacity>::createPage()
	{
		auto page = m_pages.emplace_back(std::make_shared<Page>());
		page->pageIndex = m_pages.size() - 1;
	}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Slot::Slot()
		: m_value(Type())
		, m_empty(true)
	{}

	template<typename Type, PageCapacityType PageCapacity>
	PersistentVector<Type, PageCapacity>::Slot::Slot(Type value)
		: m_value(std::move(value))
		, m_empty(false)
	{}

	template<typename Type, PageCapacityType PageCapacity>
	Type& PersistentVector<Type, PageCapacity>::Slot::get()
	{
		DEBUG_ASSERT(!m_empty);

		return m_value;
	}

	template<typename Type, PageCapacityType PageCapacity>
	const Type& PersistentVector<Type, PageCapacity>::Slot::get() const
	{
		DEBUG_ASSERT(!m_empty);

		return m_value;
	}

	template<typename Type, PageCapacityType PageCapacity>
	bool PersistentVector<Type, PageCapacity>::Slot::empty() const
	{
		return m_empty;
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Slot& PersistentVector<Type, PageCapacity>::Iterator::operator * ()
	{
		DEBUG_ASSERT(m_page != m_end);

		return (*m_page)->slots[m_index];
	}

	template<typename Type, PageCapacityType PageCapacity>
	const typename PersistentVector<Type, PageCapacity>::Slot& PersistentVector<Type, PageCapacity>::Iterator::operator * () const
	{
		DEBUG_ASSERT(m_page != m_end);

		return (*m_page)->slots[m_index];
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Slot* PersistentVector<Type, PageCapacity>::Iterator::operator -> ()
	{
		DEBUG_ASSERT(m_page != m_end);

		return &(*m_page)->slots[m_index];
	}

	template<typename Type, PageCapacityType PageCapacity>
	const typename PersistentVector<Type, PageCapacity>::Slot* PersistentVector<Type, PageCapacity>::Iterator::operator -> () const
	{
		DEBUG_ASSERT(m_page != m_end);

		return &(*m_page)->slots[m_index];
	}

	template<typename Type, PageCapacityType PageCapacity>
	typename PersistentVector<Type, PageCapacity>::Iterator& PersistentVector<Type, PageCapacity>::Iterator::operator ++()
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
	typename PersistentVector<Type, PageCapacity>::Iterator& PersistentVector<Type, PageCapacity>::Iterator::operator +(std::size_t diff)
	{
		for (std::size_t i = 0; i < diff; ++i)
		{
			if (m_page == m_end)
				break;

			++m_index;
			if (m_index >= PageCapacity)
			{
				m_index = 0;
				std::advance(m_page, 1);
			}
		}
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