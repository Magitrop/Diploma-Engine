#include "debug_logger.h"

#include <engine/dependencies/spdlog/include/spdlog/spdlog.h>

engine::debug::Logger& engine::debug::Logger::instance()
{
	static Logger logger;
	return logger;
}

void engine::debug::Logger::setLoggingPath(std::filesystem::path path)
{
	m_loggingPath = path;
}

std::filesystem::path engine::debug::Logger::loggingPath()
{
	return m_loggingPath;
}

void engine::debug::Logger::log(std::string message, Level level)
{

}
