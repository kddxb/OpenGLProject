#include "StdAfx.h"
#include "Config.h"
#include <jsoncpp/json.h>
#include <fstream>
#include "fileTool.h"

Config * Config::GetInstance()
{
	static Config instance;
	return &instance;
}

bool Config::Init(std::string binDir, std::string configRelativePath)
{
	std::string configAbsolutePath = binDir + configRelativePath;
	if (!FileTool::IsFileExist(configAbsolutePath))
	{
		return false;
	}
	m_ConfigFilePath = configAbsolutePath;

	std::ifstream inStream("config.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inStream, root);
	m_OpenglMajorVersion = root["openglVersion"]["major"].asInt();
	m_OpenglMinorVersion = root["openglVersion"]["minor"].asInt();
	m_InitWindowTitle = root["window"]["initTitle"].asString();
	m_InitWindowWidth = root["window"]["initWidth"].asInt();
	m_InitWindowHeight = root["window"]["initHeight"].asInt();
	m_LogPath = binDir + root["log"]["path"].asString();
	m_LogLevel = root["log"]["level"].asInt();

	return true;
}