#pragma once

#include <functional>
#include <vector>

namespace engine
{
	template<typename ActionType>
	class SignalEmitter final
	{
	public:
		template<typename... Args>
		void operator()(Args... args) const;

		void operator += (std::function<ActionType> action) const;
		void operator -= (std::function<ActionType> action) const;
		void clear() const;

	private:
		mutable std::vector<std::function<ActionType>> m_actions;
	};

	template<typename ActionType>
	template<typename... Args>
	void SignalEmitter<ActionType>::operator()(Args... args) const
	{
		for (auto& action : m_actions)
		{
			action(std::forward<Args>(args)...);
		}
	}

	template<typename ActionType>
	void SignalEmitter<ActionType>::operator += (std::function<ActionType> action) const
	{
		m_actions.push_back(action);
	}

	template<typename ActionType>
	void SignalEmitter<ActionType>::operator -= (std::function<ActionType> action) const
	{
		auto found = std::find(m_actions.begin(), m_actions.end(), action);
		if (found != m_actions.end())
			m_actions.erase(found);
	}

	template<typename ActionType>
	void SignalEmitter<ActionType>::clear() const
	{
		m_actions.clear();
	}
} // namespace engine