#include "stdafx.h"
#include "Program.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <GLAD/glad.h>
#include <jsoncpp/json.h>

Program::Program(const std::map<ShaderType, std::string>& shadersSource)
{
	m_ID = glCreateProgram();
	std::vector<unsigned int> toDeletedShader;
	bool successCreated = true;

	//当着色器创建失败时进行资源回收
	struct GarbageCheck {
		std::vector<unsigned int>* m_toDeletedShader;
		bool* m_SuccessCreated;
		unsigned int* m_ProgramID;
		GarbageCheck(std::vector<unsigned int>* toDeletedShader, bool* success, unsigned int* id) :m_toDeletedShader(toDeletedShader), m_SuccessCreated(success), m_ProgramID(id) {};
		~GarbageCheck()
		{
			for (unsigned int anyTypeShader : *m_toDeletedShader)
				glDeleteShader(anyTypeShader);
			if (!*m_SuccessCreated)
				glDeleteProgram(*m_ProgramID);
		}
	}garbageCheck(&toDeletedShader, &successCreated, &m_ID);

	for (const std::pair<ShaderType, std::string>& pair : shadersSource)
	{
		std::string shaderCode;
		const std::string fileName = pair.second;
		std::ifstream shaderFileStream;
		shaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shaderFileStream.open(fileName);
			std::stringstream shaderStream, fShaderStream;
			shaderStream << shaderFileStream.rdbuf();
			shaderFileStream.close();
			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "error:failed read file:" << fileName;
			successCreated = false;
			return;
		}

		const char* shaderCodeStr = shaderCode.c_str();
		//自定义着色器类型与OpenGL着色器类型对照表
		static const std::map<ShaderType, unsigned int> toGLTypes = {
			{ ShaderType::Vertex,GL_VERTEX_SHADER },
			{ ShaderType::Fragment,GL_FRAGMENT_SHADER },
			{ ShaderType::Geometry,GL_GEOMETRY_SHADER }
		};

		const unsigned int type = toGLTypes.at(pair.first);
		unsigned int anyTypeShader = glCreateShader(type);
		glShaderSource(anyTypeShader, 1, &shaderCodeStr, nullptr);
		glCompileShader(anyTypeShader);

		if (!CheckShaderCompileError(anyTypeShader, pair.first))
		{
			successCreated = false;
			return;
		}
		glAttachShader(m_ID, anyTypeShader);
		//记录待删除的着色器
		toDeletedShader.push_back(anyTypeShader);
	}

	Link();

	if (!CheckProgramLinkError())
	{
		successCreated = false;
		return;
	}
}

Program::Program(const std::string& vertexPath, const std::string& fragmentPath)
	: Program(std::map<ShaderType, std::string>{ {ShaderType::Vertex, vertexPath}, { ShaderType::Fragment,fragmentPath }})
{
}

Program::Program(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	: Program(std::map<ShaderType, std::string>{ {ShaderType::Vertex, vertexPath}, { ShaderType::Geometry,geometryPath }, { ShaderType::Fragment,fragmentPath }})
{
}

Program::~Program()
{
	if (glIsProgram(m_ID))
	{
		glDeleteShader(m_ID);
	}
}

void Program::Use()const
{
	glUseProgram(this->m_ID);
}

void Program::Link()const
{
	glLinkProgram(this->m_ID);
}

void Program::SetUniform(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->m_ID, name.c_str()), int(value));
}

void Program::SetUniform(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(this->m_ID, name.c_str()), value);
}

void Program::SetUniform(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(this->m_ID, name.c_str()), value);
}

void Program::SetUniform(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, &value[0]);
}
void Program::SetUniform(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->m_ID, name.c_str()), x, y);
}

void Program::SetUniform(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, &value[0]);
}
void Program::SetUniform(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->m_ID, name.c_str()), x, y, z);
}

void Program::SetUniform(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, &value[0]);
}
void Program::SetUniform(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->m_ID, name.c_str()), x, y, z, w);
}

void Program::SetUniform(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::SetUniform(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::SetUniform(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool Program::CheckShaderCompileError(unsigned int shaderId, const ShaderType& shaderType)
{
	const static std::map<ShaderType, std::string> shaderTypeStrMap =
	{
		{ ShaderType::Vertex,"Vertex" },
		{ ShaderType::Fragment, "Fragment" },
		{ ShaderType::Geometry,"Geometry" }
	};

	const int infoLogSize = 1024;
	int success;
	char infoLog[infoLogSize];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		m_ID = 0;
		glGetShaderInfoLog(shaderId, infoLogSize, nullptr, infoLog);
		std::string errInfo = std::string("error::shader compile error of type: ") + shaderTypeStrMap.at(shaderType) + "\ndetail: " + infoLog;
		std::cout << errInfo;
		return false;
	}
	return true;
}

bool Program::CheckProgramLinkError()
{
	const int infoLogSize = 1024;
	int success;
	char infoLog[infoLogSize];
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, infoLogSize, nullptr, infoLog);
		std::string errInfo = std::string("error::program link error,detail: ") + "\n" + infoLog;
		std::cout << errInfo;
		return false;
	}
	return true;
}

std::map<ProgramType, std::unique_ptr<Program>> ProgramFactory::m_Programs;
bool ProgramFactory::m_Inited = false;

bool ProgramFactory::Init()
{
	try
	{
		std::ifstream inStream("shaders/programs.json");
		Json::Reader reader;
		Json::Value root;
		reader.parse(inStream, root);
		Json::Value programs = root["programs"];
		for (int programIndex = 0; programIndex < int(programs.size()); ++programIndex)
		{
			Json::Value program = programs[programIndex];
			std::string programName = program["name"].asString();
			ProgramType programType = static_cast<ProgramType>(program["type"].asInt());
			Json::Value shaders = program["shaders"];
			std::map<ShaderType, std::string> shadersSource;
			for (int shaderIndex = 0; shaderIndex < int(shaders.size()); ++shaderIndex)
			{
				Json::Value shader = shaders[shaderIndex];
				ShaderType shaderType = static_cast<ShaderType>(shader["type"].asInt());
				std::string shaderPath = shader["path"].asString();
				shadersSource.insert(std::make_pair(shaderType, shaderPath));
			}
			std::unique_ptr<Program> upProgram(new Program(shadersSource));
			upProgram->SetName(programName);
			m_Programs.insert(std::make_pair(programType, std::move(upProgram)));
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		return false;
	}
	return true;
}

Program * ProgramFactory::Get(ProgramType programType)
{
	if (!m_Inited)
	{
		Init();
		m_Inited = true;
	}
	auto it = m_Programs.find(programType);
	if (it != m_Programs.end())
	{
		return it->second.get();
	}
	return nullptr;
}
