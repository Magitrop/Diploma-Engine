#pragma once

#include <memory>

#include <engine/internal/helpers/non_copyable.h>
#include <engine/internal/helpers/non_movable.h>

namespace engine
{
	class IRuntimePipeline;
	class Runtime final : NonCopyable, NonMovable
	{
		// friends
	private:
		friend class Main;

		// members
	private:
		explicit Runtime();

	public:
		~Runtime();

		bool isRunning() const;

	private:
		void initialize();
		void run();

		void setPipeline(IRuntimePipeline* pipeline);

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine
