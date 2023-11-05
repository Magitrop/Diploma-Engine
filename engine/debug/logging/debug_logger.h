#pragma once

#include <string>

namespace engine
{
	namespace debug
	{
		class Logger final
		{
		public:
			enum class Level
			{
				Trace,
				Debug,
				Info,
				Warning,
				Error,
				Fatal
			};

		private:
			Logger();
			Logger(Logger&) = delete;
			Logger(Logger&&) = delete;
			Logger& operator = (Logger&) = delete;
			Logger& operator = (Logger&&) = delete;

		public:
			static Logger& instance();

			void log(std::string message, Level level);

		private:
			std::string m_logsPath;
		};
	} // namespace debug
} // namespace engine