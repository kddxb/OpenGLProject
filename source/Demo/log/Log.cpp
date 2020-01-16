#include "StdAfx.h"
#include "Log.h"
#include "fileTool.h"
#include "Config.h"
#include <iostream>
#include <time.h>

namespace
{
	std::string getLogLevelString(const LogLevel& logLevel)
	{
		switch (logLevel)
		{
		case LogLevel::Info:
			return "Info";
		case LogLevel::Warn:
			return "Warn";
		case LogLevel::Error:
			return "Error";
		default:
			return "UnKnown";
		}
	}

	std::string getFormatTime()
	{
		char buff[20];          // sizeof("2018-04-19 19:49:23") == 20;
		time_t now = time(NULL);
		tm time;
		localtime_s(&time, &now);
		strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", &time);
		return buff;
	}
}

Log * Log::GetInstance()
{
	static Log instance;
	return &instance;
}

void Log::Write(std::string str, LogLevel logLevel)
{
	if (int(logLevel) >= Config::GetInstance()->GetLogLevel())
	{
		char buffer[256];
		sprintf_s(buffer, "[%s]@%s    %s\n", getLogLevelString(logLevel).c_str(), getFormatTime().c_str(), str.c_str());
		std::cout << buffer << std::endl;
		FileTool::WriteFile(Config::GetInstance()->GetLogPath(), buffer);
	}
}