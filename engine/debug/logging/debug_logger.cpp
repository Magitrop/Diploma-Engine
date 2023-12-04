#include "debug_logger.h"

#if USE_LOGGER
#include <iomanip>
#include <sstream>

#include <engine/dependencies/spdlog/include/spdlog/sinks/basic_file_sink.h>

namespace engine
{
	class Logger::Internal final
	{
	public:
		Internal(std::filesystem::path directory);
		~Internal() = default;

		void setMinimumLevel(Level level);
		void immediatelyLogToFile(bool enable);
		void useFileLogging(bool enable);
		void useConsoleLogging(bool enable);

		spdlog::level::level_enum getLevel(Level level);
		void logInternal(std::string message, Level level);

	private:
		static std::shared_ptr<Logger> m_instance;

		std::shared_ptr<spdlog::logger> m_fileLogger;

		Level m_minLevel = Level::Debug;
		bool m_immediatelyLogToFile = false;
		bool m_useFileLogging = false;
		bool m_useConsoleLogging = true;
		std::filesystem::path m_loggingPath;

		constexpr static const char* m_logFilenameTemplate = "log_%d_%m_%Y_%H_%M_%S.txt"; // day, month, year, hours, minutes, seconds
	};

	Logger::Internal::Internal(std::filesystem::path directory)
		: m_loggingPath(directory)
	{
		// an annoying way of translating the current datetime into a string
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::ostringstream filename;
		filename << std::put_time(std::localtime(&now), m_logFilenameTemplate);

		m_loggingPath += filename.str();
	}

	void Logger::Internal::setMinimumLevel(Level level)
	{
		m_minLevel = level;
		if (m_fileLogger) m_fileLogger->set_level(getLevel(level));
		spdlog::set_level(getLevel(level));
	}

	void Logger::Internal::immediatelyLogToFile(bool enable)
	{
		m_immediatelyLogToFile = enable;
	}

	void Logger::Internal::useFileLogging(bool enable)
	{
		m_useFileLogging = enable;

		// create log file only if needed
		if (m_useFileLogging && m_fileLogger == nullptr)
		{
			m_fileLogger = spdlog::basic_logger_mt("Logger", m_loggingPath.generic_string());
		}
	}

	void Logger::Internal::useConsoleLogging(bool enable)
	{
		m_useConsoleLogging = enable;
	}

	spdlog::level::level_enum Logger::Internal::getLevel(Level level)
	{
		switch (level)
		{
		case Level::Trace:
			return spdlog::level::level_enum::trace;
		case Level::Debug:
			return spdlog::level::level_enum::debug;
		case Level::Info:
			return spdlog::level::level_enum::info;
		case Level::Warning:
			return spdlog::level::level_enum::warn;
		case Level::Error:
			return spdlog::level::level_enum::err;
		case Level::Fatal:
			return spdlog::level::level_enum::critical;
		default:
			return spdlog::level::level_enum::off;
		}
	}

	void Logger::Internal::logInternal(std::string message, Level level)
	{
		if (level < m_minLevel)
			return;

		switch (level)
		{
		case Level::Trace:
			if (m_useFileLogging) m_fileLogger->log(spdlog::level::level_enum::trace, message);
			if (m_useConsoleLogging) spdlog::log(spdlog::level::level_enum::trace, message);
			break;
		case Level::Debug:
			if (m_useFileLogging) m_fileLogger->log(spdlog::level::level_enum::debug, message);
			if (m_useConsoleLogging) spdlog::log(spdlog::level::level_enum::debug, message);
			break;
		case Level::Info:
			if (m_useFileLogging) m_fileLogger->log(spdlog::level::level_enum::info, message);
			if (m_useConsoleLogging) spdlog::log(spdlog::level::level_enum::info, message);
			break;
		case Level::Warning:
			if (m_useFileLogging) m_fileLogger->log(spdlog::level::level_enum::warn, message);
			if (m_useConsoleLogging) spdlog::log(spdlog::level::level_enum::warn, message);
			break;
		case Level::Error:
			if (m_useFileLogging) m_fileLogger->log(spdlog::level::level_enum::err, message);
			if (m_useConsoleLogging) spdlog::log(spdlog::level::level_enum::err, message);
			break;
		case Level::Fatal:
			if (m_useFileLogging) m_fileLogger->log(spdlog::level::level_enum::critical, message);
			if (m_useConsoleLogging) spdlog::log(spdlog::level::level_enum::critical, message);
			break;
		default:
			break;
		}

		if (m_useFileLogging && m_immediatelyLogToFile)
			m_fileLogger->flush();
	}

	Logger::Logger(std::filesystem::path directory)
	{
		m_internal = std::make_unique<Internal>(directory);
	}

	Logger::~Logger()
	{}

	void Logger::setMinimumLevel(Level level)
	{
		m_internal->setMinimumLevel(level);
	}

	void Logger::immediatelyLogToFile(bool enable)
	{
		m_internal->immediatelyLogToFile(enable);
	}

	void Logger::useFileLogging(bool enable)
	{
		m_internal->useFileLogging(enable);
	}

	void Logger::useConsoleLogging(bool enable)
	{
		m_internal->useConsoleLogging(enable);
	}

	spdlog::level::level_enum Logger::getLevel(Level level)
	{
		return m_internal->getLevel(level);
	}

	void Logger::logInternal(std::string message, Level level)
	{
		m_internal->logInternal(message, level);
	}

	std::shared_ptr<Logger> Logger::m_instance = nullptr;
	std::shared_ptr<Logger> Logger::instance()
	{
		return m_instance;
	}
} // namespace engine
#endif // #if USE_LOGGER