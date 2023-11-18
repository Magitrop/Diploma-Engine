#pragma once

#include <memory>

namespace engine
{
	template<typename GraphicAPI>
	struct RuntimePipelineContext
	{
		typedef GraphicAPI GraphicAPI;
	};

	class IRuntimePipeline
	{
		// friends
	private:
		friend class Runtime;

		// members
	public:
		bool isRunning();

	protected:
		virtual void initialize() = 0;
		virtual void finalize() = 0;
		virtual void run() = 0;

		bool m_isRunning = false;
	};
} // namespace engine
