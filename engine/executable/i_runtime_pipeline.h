#pragma once

#include <memory>

namespace engine
{
	class IRuntimePipeline
	{
		// friends
	private:
		friend class Runtime;

		// members
	public:
		bool isRunning();

	protected:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;
		virtual void run() = 0;

		bool m_isRunning = false;
	};
} // namespace engine
