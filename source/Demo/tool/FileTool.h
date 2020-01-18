#pragma once

#include <string>

class FileTool
{
private:
	FileTool() = delete;
	FileTool& operator=(const FileTool&) = delete;
	FileTool(const FileTool&) = delete;
	~FileTool() = delete;

public:
	//获取某文件文本内容
	static std::string ReadFile(const std::string& fileName);

	//向指定文件中写入指定内容数据
	static void WriteFile(const std::string& fileName, const std::string& content, bool append = true);

	//拷贝文件
	static void Copy(std::string src, std::string dst);

	//文件是否存在
	static bool IsFileExist(const std::string& fileName);
};