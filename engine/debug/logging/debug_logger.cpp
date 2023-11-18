#include "debug_logger.h"

#include <engine/dependencies/spdlog/include/spdlog/spdlog.h>

engine::Logger& engine::Logger::instance()
{
	static Logger logger;
	return logger;
}

void engine::Logger::setLoggingPath(std::filesystem::path path)
{
	m_loggingPath = path;
}

std::filesystem::path engine::Logger::loggingPath()
{
	return m_loggingPath;
}

void engine::Logger::log(std::string message, Level level)
{

}
