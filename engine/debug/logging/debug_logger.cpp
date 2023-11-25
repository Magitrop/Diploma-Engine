#include "debug_logger.h"

#if USE_LOGGER
#include <iomanip>
#include <sstream>

#include <engine/dependencies/spdlog/include/spdlog/sinks/basic_file_sink.h>

namespace engine
{
	Logger::Logger(std::filesystem::path directory)
		: m_loggingPath(directory)
	{
		// an annoying way of translating the current datetime into a string
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::ostringstream filename;
		filename << std::put_time(std::localtime(&now), m_logFilenameTemplate);

		m_loggingPath += filename.str();
	}

	std::shared_ptr<Logger> Logger::instance()
	{
		return m_instance;
	}

	void Logger::setMinimumLevel(Level level)
	{
		m_minLevel = level;
		if (m_fileLogger) m_fileLogger->set_level(getLevel(level));
		spdlog::set_level(getLevel(level));
	}

	void Logger::immediatelyLogToFile(bool enable)
	{
		m_immediatelyLogToFile = enable;
	}

	void Logger::useFileLogging(bool enable)
	{
		m_useFileLogging = enable;

		// create log file only if needed
		if (m_useFileLogging && m_fileLogger == nullptr)
		{
			m_fileLogger = spdlog::basic_logger_mt("Logger", m_loggingPath.generic_string());
		}
	}

	void Logger::useConsoleLogging(bool enable)
	{
		m_useConsoleLogging = enable;
	}

	spdlog::level::level_enum Logger::getLevel(Level level)
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

	void Logger::logInternal(std::string message, Level level)
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

	std::shared_ptr<Logger> Logger::m_instance = nullptr;
} // namespace engine
#endif // #if USE_LOGGER