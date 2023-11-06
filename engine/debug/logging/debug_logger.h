#pragma once

#include <filesystem>
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
				Trace,		// very detailed information
				Debug,		// detailed information
				Info,		// informational generalized messages or hints
				Warning,	// noticeable warnings
				Error,		// major errors
				Fatal		// severe errors that should cause a crash
			};

		private:
			Logger() = default;

		public:
			static Logger& instance();

			void setLoggingPath(std::filesystem::path path);
			std::filesystem::path loggingPath();

			void log(std::string message, Level level);

		private:
			std::filesystem::path m_loggingPath;
		};
	} // namespace debug

	using Logger = engine::debug::Logger;
} // namespace engine