#include "memory_guard.h"

#include <new>

#ifdef USE_MEMORY_GUARD

namespace engine
{
	void* MemoryGuardAllocator::allocate(std::size_t size)
	{
		void* result;

		if (size == 0)
			size = 1;

		while ((result = std::malloc(size)) == nullptr)
		{
			std::new_handler handler = std::get_new_handler();
			if (!handler)
				throw std::bad_alloc{};
			handler();
		}

		m_allocations[result] = MemoryGuardStack::front();
		return result;
	}

	void MemoryGuardAllocator::deallocate(void* ptr)
	{
		m_allocations.erase(ptr);
		std::free(ptr);
	}

	const MemoryGuardAllocator::AllocationsMap& MemoryGuardAllocator::getAllocations()
	{
		return m_allocations;
	}

	void MemoryGuardStack::push(const char* function, const char* file, std::size_t line)
	{
		if (m_stackPosition >= STACK_MAX_DEPTH)
			throw std::exception{};

		auto& context = m_stack[m_stackPosition];
		context.m_function = function;
		context.m_file = file;
		context.m_line = line;

		++m_stackPosition;
	}

	void MemoryGuardStack::pop()
	{
		if (m_stackPosition <= 0)
			throw std::exception{};
		--m_stackPosition;
	}

	const MemoryGuardContext& MemoryGuardStack::front()
	{
		return m_stack[m_stackPosition - 1];
	}

	MemoryGuardScope::MemoryGuardScope(const char* function, const char* file, std::size_t line)
	{
		MemoryGuardStack::push(function, file, line);
	}

	MemoryGuardScope::~MemoryGuardScope()
	{
		MemoryGuardStack::pop();
	}

	// static variables

	MemoryGuardAllocator::AllocationsMap MemoryGuardAllocator::m_allocations(MemoryGuardAllocator::m_maxAllocations);
	thread_local MemoryGuardContext MemoryGuardStack::m_stack[STACK_MAX_DEPTH];
	thread_local std::size_t MemoryGuardStack::m_stackPosition = 0;
} // namespace engine

void* ::operator new(std::size_t size)
{
	return engine::MemoryGuardAllocator::allocate(size);
}

void ::operator delete(void* ptr)
{
	engine::MemoryGuardAllocator::deallocate(ptr);
}

#endif // #ifdef USE_MEMORY_GUARD