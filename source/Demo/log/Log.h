#pragma once
#include <string>

enum class LogLevel
{
	Info,
	Warn,
	Error
};

class Log
{
private:
	//½ûÖ¹¸´ÖÆ
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
	Log() = default;

public:
	static Log* GetInstance();
	void Write(std::string str, LogLevel logLevel);

};