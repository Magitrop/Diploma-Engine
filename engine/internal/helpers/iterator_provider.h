#pragma once

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/assert.h>

namespace engine
{
	// Provides access to iterators of a certain class that may have several of them.
	template<typename Owner, typename Iterator>
	class IteratorProvider final
	{
	public:
		explicit IteratorProvider(const Owner* owner);

		const Iterator begin() const;
		const Iterator end() const;

	private:
		const Owner* m_owner;
	};

	template<typename Owner, typename Iterator>
	IteratorProvider<Owner, Iterator>::IteratorProvider(const Owner* owner)
		: m_owner(owner)
	{
		DEBUG_ASSERT(m_owner != nullptr);
	}

	template<typename Owner, typename Iterator>
	const Iterator IteratorProvider<Owner, Iterator>::begin() const
	{
		return Iterator::begin(m_owner);
	}

	template<typename Owner, typename Iterator>
	const Iterator IteratorProvider<Owner, Iterator>::end() const
	{
		return Iterator::end(m_owner);
	}
} // namespace engine
