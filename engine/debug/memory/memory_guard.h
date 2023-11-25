#pragma once

#include <cstddef>
#include <unordered_map>

#if USE_MEMORY_GUARD
namespace engine
{
	struct MemoryGuardContext
	{
		const char* m_function;
		const char* m_file;
		std::size_t m_line;
	};
	class MemoryGuardStack
	{
	public:
		static void push(const char* function, const char* file, std::size_t line);
		static void pop();
		static const MemoryGuardContext& front();

	private:
		static const std::size_t STACK_MAX_DEPTH = 512;

		static thread_local MemoryGuardContext m_stack[STACK_MAX_DEPTH];
		static thread_local std::size_t m_stackPosition;
	};

	struct MemoryGuardScope
	{
		MemoryGuardScope(const char* function, const char* file, std::size_t line);
		~MemoryGuardScope();
	};

	template<typename T>
	class MemoryGuardDebugAllocator : public std::allocator<T>
	{
	public:
		T* allocate(std::size_t size)
		{
			T* result;

			if (size == 0)
				size = 1;

			while ((result = static_cast<T*>(std::malloc(sizeof(T) * size))) == nullptr)
			{
				std::new_handler handler = std::get_new_handler();
				if (!handler)
					throw std::bad_alloc{};
				handler();
			}

			return result;
		}

		void deallocate(T* p, std::size_t n)
		{
			std::free(p);
		}
	};

	class MemoryGuardAllocator
	{
	private:
		using Hasher = std::hash<void*>;
		using Comparator = std::equal_to<void*>;
		using AllocatorPair = std::pair<void* const, MemoryGuardContext>;
		using Allocator = MemoryGuardDebugAllocator<AllocatorPair>;
		using AllocationsMap = std::unordered_map<void*, MemoryGuardContext, Hasher, Comparator, Allocator>;

	public:
		static void* allocate(std::size_t size);
		static void deallocate(void* ptr);

		static const AllocationsMap& getAllocations();

	private:
		const static std::size_t m_maxAllocations = 1 << 16;
		static AllocationsMap m_allocations;
	};
} // namespace engine
#define MEMORY_GUARD engine::MemoryGuardScope _MEMORY_GUARD_(__func__, __FILE__, __LINE__)
#else
#define MEMORY_GUARD
#endif // #if USE_MEMORY_GUARD