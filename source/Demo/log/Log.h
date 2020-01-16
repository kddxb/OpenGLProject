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
	//��ֹ����
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
	Log() = default;

public:
	static Log* GetInstance();
	void Write(std::string str, LogLevel logLevel);

};