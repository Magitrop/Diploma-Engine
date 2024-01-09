#pragma once

#include <functional>

namespace engine
{
	// Executes all given functions until one of them returns false.
	template<typename Owner>
	class ScopedSequentialExecutor final
	{
	public:
		ScopedSequentialExecutor(Owner* owner, bool interruptAfterFail = true)
			: m_owner(owner)
			, m_interruptAfterFail(interruptAfterFail)
		{}

		template<typename Func>
		bool operator()(Func function)
		{
			if (!m_shouldContinue)
				return false;

			bool result = std::bind(function, m_owner)();
			if (!result && m_interruptAfterFail)
				m_shouldContinue = false;

			return result;
		}

		bool result()
		{
			return m_shouldContinue;
		}

	private:
		Owner* m_owner;
		bool m_interruptAfterFail;
		bool m_shouldContinue = true;
	};
} // namespace engine
