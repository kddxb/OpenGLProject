#include "StdAfx.h"
#include "FileTool.h"
#include <fstream>
#include <sstream>
#include <io.h>
#include <direct.h>
#include <iostream>

std::string FileTool::ReadFile(const std::string& fileName)
{
	std::string src;
	std::ifstream file;
	file.open(fileName);
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	file.close();
	src = fileStream.str();
	return src;
}

void FileTool::WriteFile(const std::string& fileName, const std::string& content, bool append/* = true*/)
{
	if (append)
	{
		std::ofstream outfile(fileName, std::ios::app);
		outfile << content;
		outfile.close();
	}
	else
	{
		std::ofstream outfile(fileName, std::ios::trunc);
		outfile << content;
		outfile.close();
	}
}

void FileTool::Copy(std::string src, std::string dst)
{
	using namespace std;
	ifstream in(src.c_str(), ios::binary);
	ofstream out(dst.c_str(), ios::binary);
	if (!in.is_open()) {
		std::cout << "error open file " << src << std::endl;
	}
	if (!out.is_open()) {
		cout << "error open file " << dst << endl;
	}
	if (src == dst) {
		cout << "the src file can't be same with dst file" << endl;
	}
	char buf[2048];
	long long totalBytes = 0;
	while (in)
	{
		//read从in流中读取2048字节，放入buf数组中，同时文件指针向后移动2048字节
		//若不足2048字节遇到文件结尾，则以实际提取字节读取。
		in.read(buf, 2048);
		//gcount()用来提取读取的字节数，write将buf中的内容写入out流。
		out.write(buf, in.gcount());
		totalBytes += in.gcount();
	}
	in.close();
	out.close();
}


bool FileTool::IsFileExist(const std::string & fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}