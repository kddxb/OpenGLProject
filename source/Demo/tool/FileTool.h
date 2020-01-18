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
	//��ȡĳ�ļ��ı�����
	static std::string ReadFile(const std::string& fileName);

	//��ָ���ļ���д��ָ����������
	static void WriteFile(const std::string& fileName, const std::string& content, bool append = true);

	//�����ļ�
	static void Copy(std::string src, std::string dst);

	//�ļ��Ƿ����
	static bool IsFileExist(const std::string& fileName);
};