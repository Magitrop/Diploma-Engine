#include "debug_logger.h"

engine::debug::Logger& engine::debug::Logger::instance()
{
	static Logger logger;
	return logger;
}

void engine::debug::Logger::log(std::string message, Level level)
{
}
