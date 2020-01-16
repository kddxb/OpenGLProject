#include "StdAfx.h"
#include "Config.h"

int main11();

int main(int argc,char** argv)
{
	std::string exeFullName = argv[0];
	std::string exePath = exeFullName.substr(0, exeFullName.find_last_of('\\') + 1);
	Config::GetInstance()->Init(exePath, "config.json");

    return main11();
}