#ifndef LOGGER_H
#define LOGGER_H
#include <string>
class Logger
{
public:
	Logger();
	~Logger();
	void Info(std::string message);
	void Error(std::string message);
};
#endif 
