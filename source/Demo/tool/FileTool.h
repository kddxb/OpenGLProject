#pragma once

#include <string>

class FileTool
{
public:
	//��ȡĳ�ļ��ı�����
	static std::string ReadFile(const std::string& fileName);

	//��ָ���ļ���д��ָ����������
	static void WriteFile(const std::string& fileName, const std::string& content, bool append = true);

	//�����ļ�
	static void Copy(std::string src, std::string dst);
};