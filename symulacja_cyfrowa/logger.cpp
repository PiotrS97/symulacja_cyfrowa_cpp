#include "logger.h"
#include <iostream>

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Info(std::string message)
{
	std::cout << "[info]: " << message;
}

void Logger::Error(std::string message)
{
	std::cout << "[Error]: " << message;
}