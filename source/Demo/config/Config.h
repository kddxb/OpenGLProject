#pragma once
#include <string>

class Config
{
private:
	//½ûÖ¹¸´ÖÆ
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;
	Config() = default;

public:
	static Config* GetInstance();
	bool Init(std::string binDir, std::string configPath);

	int GetOpenglMajorVersion()const { return m_OpenglMajorVersion; }
	int GetOpenglMinorVersion()const { return m_OpenglMinorVersion; }
	const std::string GetInitWindowTitle()const { return m_InitWindowTitle; }
	int GetInitWindowWidth()const { return m_InitWindowWidth; }
	int GetInitWindowHeight()const { return m_InitWindowHeight; }
	const std::string GetLogPath()const { return m_LogPath; }
	int GetLogLevel()const { return m_LogLevel; }

private:
	std::string m_ConfigFilePath;
	int m_OpenglMajorVersion = 0;
	int m_OpenglMinorVersion = 0;
	std::string m_InitWindowTitle;
	int m_InitWindowWidth = 0;
	int m_InitWindowHeight = 0;
	std::string m_LogPath;
	int m_LogLevel = 0;
};