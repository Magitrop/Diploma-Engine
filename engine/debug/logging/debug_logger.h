#pragma once

#if USE_LOGGER
#include <filesystem>
#include <string>

#include <engine/dependencies/spdlog/include/spdlog/spdlog.h>

namespace engine
{
	class Logger final
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;

		// members
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
		explicit Logger(std::filesystem::path directory);

	public:
		[[nodiscard]] static std::shared_ptr<Logger> instance();

		void setMinimumLevel(Level level);
		void immediatelyLogToFile(bool enable); // immediately flush logs to file
		void useFileLogging(bool enable);
		void useConsoleLogging(bool enable);

		// formatted log
		template<typename... Args> void log(std::string message, Level level = Level::Info, Args... args);

	private:
		spdlog::level::level_enum getLevel(Level level);
		void logInternal(std::string message, Level level);

		static std::shared_ptr<Logger> m_instance;

		std::shared_ptr<spdlog::logger> m_fileLogger;

		Level m_minLevel = Level::Debug;
		bool m_immediatelyLogToFile = false;
		bool m_useFileLogging = false;
		bool m_useConsoleLogging = true;
		std::filesystem::path m_loggingPath;

		constexpr static const char* m_logFilenameTemplate = "log_%d_%m_%Y_%H_%M_%S.txt"; // day, month, year, hours, minutes, seconds
	};

	template<typename ...Args>
	void Logger::log(std::string message, Level level, Args... args)
	{
		logInternal(fmt::vformat(message, fmt::make_format_args(args...)), level);
	}
} // namespace engine

#define TRACE_LOG(msg, ...) engine::Logger::instance()->log(msg, engine::Logger::Level::Trace, __VA_ARGS__)
#define DEBUG_LOG(msg, ...) engine::Logger::instance()->log(msg, engine::Logger::Level::Debug, __VA_ARGS__)
#define INFO_LOG(msg, ...) engine::Logger::instance()->log(msg, engine::Logger::Level::Info, __VA_ARGS__)
#define WARNING_LOG(msg, ...) engine::Logger::instance()->log(msg, engine::Logger::Level::Warning, __VA_ARGS__)
#define ERROR_LOG(msg, ...) engine::Logger::instance()->log(msg, engine::Logger::Level::Error, __VA_ARGS__)
#define FATAL_LOG(msg, ...) engine::Logger::instance()->log(msg, engine::Logger::Level::Fatal, __VA_ARGS__)
#else
#define TRACE_LOG(msg, ...)
#define DEBUG_LOG(msg, ...)
#define INFO_LOG(msg, ...)
#define WARNING_LOG(msg, ...)
#define ERROR_LOG(msg, ...)
#define FATAL_LOG(msg, ...)
#endif // #if USE_LOGGER