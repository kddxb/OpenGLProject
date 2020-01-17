#include "StdAfx.h"
#include "Program.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <GLAD/glad.h>
#include <jsoncpp/json.h>
#include "texture.h"
#include "Log.h"

Program::Program(const std::map<ShaderType, std::string>& shadersSource, bool isFromFile)
{
    m_ID = glCreateProgram();
    std::vector<unsigned int> shadersToBeDeleted;
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
    }garbageCheck(&shadersToBeDeleted, &successCreated, &m_ID);

    for (const std::pair<ShaderType, std::string>& pair : shadersSource)
    {
        std::string shaderCode;

        if (!isFromFile)
        {
            shaderCode = pair.second;
        }
        else
        {
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
				Log::GetInstance()->Write("error:failed read file:", LogLevel::Error);
                successCreated = false;
                return;
            }
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
        shadersToBeDeleted.push_back(anyTypeShader);
    }

    Link();

    if (!CheckProgramLinkError())
    {
        successCreated = false;
        return;
    }
}

Program::Program(const std::string& vertexPath, const std::string& fragmentPath, bool isFromFile)
    : Program(std::map<ShaderType, std::string>{ {ShaderType::Vertex, vertexPath}, { ShaderType::Fragment,fragmentPath }}, isFromFile)
{
}

Program::Program(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, bool isFromFile)
    : Program(std::map<ShaderType, std::string>{ {ShaderType::Vertex, vertexPath}, { ShaderType::Geometry,geometryPath }, { ShaderType::Fragment,fragmentPath }}, isFromFile)
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

void Program::SetUniform(const std::string &uniformName, bool value) const
{
    SetUniform(uniformName, int(value));
}

void Program::SetUniform(const std::string &uniformName, int value) const
{
    glProgramUniform1i(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), value);
}

void Program::SetUniform(const std::string &uniformName, float value) const
{
    glProgramUniform1f(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), value);
}

void Program::SetUniform(const std::string &uniformName, const glm::vec2 &value) const
{
    glProgramUniform2fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), 1, &value[0]);
}
void Program::SetUniform(const std::string &uniformName, float x, float y) const
{
    glProgramUniform2f(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), x, y);
}

void Program::SetUniform(const std::string &uniformName, const glm::vec3 &value) const
{
    glProgramUniform3fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), 1, &value[0]);
}
void Program::SetUniform(const std::string &uniformName, float x, float y, float z) const
{
    glProgramUniform3f(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), x, y, z);
}

void Program::SetUniform(const std::string &uniformName, const glm::vec4 &value) const
{
    glProgramUniform4fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), 1, &value[0]);
}
void Program::SetUniform(const std::string &uniformName, float x, float y, float z, float w) const
{
    glProgramUniform4f(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), x, y, z, w);
}

void Program::SetUniform(const std::string &uniformName, const glm::mat2 &mat) const
{
    glProgramUniformMatrix2fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::SetUniform(const std::string &uniformName, const glm::mat3 &mat) const
{
    glProgramUniformMatrix3fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::SetUniform(const std::string &uniformName, const glm::mat4 &mat) const
{
    glProgramUniformMatrix4fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::SetUniform(const std::string & uniformName, Texture1D * pTexture)const
{
	SetUniform(uniformName, pTexture->GetTextureUnit());
}

void Program::SetUniform(const std::string & uniformName, Texture2D * pTexture)const
{
	SetUniform(uniformName, pTexture->GetTextureUnit());
}

void Program::SetUniform(const std::string &uniformName, const std::vector<bool> &values) const
{
    const std::vector<int> ivalues(values.begin(), values.end());
    SetUniform(uniformName, ivalues);
}

void Program::SetUniform(const std::string &uniformName, const std::vector<int> &values) const
{
    glProgramUniform1iv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), values.size(), values.data());
}

void Program::SetUniform(const std::string &uniformName, const std::vector<float> &values) const
{
    glProgramUniform1fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), values.size(), values.data());
}

void Program::SetUniform(const std::string &uniformName, const std::vector<glm::vec2> &values) const
{
    glProgramUniform2fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), values.size(), &values.front()[0]);
}

void Program::SetUniform(const std::string &uniformName, const std::vector<glm::vec3>& values) const
{
    glProgramUniform3fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), values.size(), &values.front()[0]);
}

void Program::SetUniform(const std::string &uniformName, const std::vector<glm::vec4> &values) const
{
    glProgramUniform4fv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), values.size(), &values.front()[0]);
}

bool Program::GetUniform(const std::string & uniformName, bool & value, bool checkSize) const
{
    int iv;
    GetUniform(uniformName, iv);
    value = iv != 0;
    return true;
}

bool Program::GetUniform(const std::string & uniformName, int & value, bool checkSize) const
{
    if (checkSize)
    {
        glGetnUniformiv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), sizeof(value), &value);
    }
    else
    {
        glGetUniformiv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), &value);
    }
    return true;
}

bool Program::GetUniform(const std::string & uniformName, float & value, bool checkSize) const
{
    if (checkSize)
    {
        glGetnUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), sizeof(value), &value);
    }
    else
    {
        glGetUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), &value);
    }
    return true;
}

bool Program::GetUniform(const std::string & uniformName, glm::vec2 & value, bool checkSize) const
{
    if (checkSize)
    {
        glGetnUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), sizeof(value), &value[0]);
    }
    else
    {
        glGetUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), &value[0]);
    }
    return true;
}

bool Program::GetUniform(const std::string & uniformName, float & x, float & y, bool checkSize) const
{
    glm::vec2 vv;
    GetUniform(uniformName, vv);
    x = vv.x;
    y = vv.y;
    return true;
}

bool Program::GetUniform(const std::string & uniformName, glm::vec3 & value, bool checkSize) const
{
    if (checkSize)
    {
        glGetnUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), sizeof(value), &value[0]);
    }
    else
    {
        glGetUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), &value[0]);
    }
    return true;
}

bool Program::GetUniform(const std::string & uniformName, float & x, float & y, float & z, bool checkSize) const
{
    glm::vec3 vv;
    GetUniform(uniformName, vv);
    x = vv.x;
    y = vv.y;
    z = vv.z;
    return true;
}

bool Program::GetUniform(const std::string & uniformName, glm::vec4 & value, bool checkSize) const
{
    if (checkSize)
    {
        glGetnUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), sizeof(value), &value[0]);
    }
    else
    {
        glGetUniformfv(this->m_ID, glGetUniformLocation(this->m_ID, uniformName.c_str()), &value[0]);
    }
    return true;
}

bool Program::GetUniform(const std::string & uniformName, float & x, float & y, float & z, float & w, bool checkSize) const
{
    glm::vec4 vv;
    GetUniform(uniformName, vv);
    x = vv.x;
    y = vv.y;
    z = vv.z;
    w = vv.w;
    return true;
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
		Log::GetInstance()->Write(errInfo, LogLevel::Error);
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
		Log::GetInstance()->Write(errInfo, LogLevel::Error);
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
        std::ifstream inStream("res/programs.json");
        Json::Reader reader;
        Json::Value root;
        reader.parse(inStream, root);
        Json::Value programs = root["programs"];
        for (int programIndex = 0; programIndex < int(programs.size()); ++programIndex)
        {
            Json::Value program = programs[programIndex];
            std::string programName = program["uniformName"].asString();
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
		Log::GetInstance()->Write(e.what(), LogLevel::Error);
        return false;
    }
    catch (...)
    {
		Log::GetInstance()->Write("unknown exception", LogLevel::Error);
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
