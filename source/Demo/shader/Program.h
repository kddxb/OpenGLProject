#pragma once
#include <vector>
#include <map>
#include <memory>
#include <GLM/glm.hpp>
#include <string>

enum class ShaderType
{
	Vertex = 0,
	Fragment = 1,
	Geometry = 2,
};

enum class ProgramType
{
	Default = 0,
};

class Texture1D;
class Texture2D;

class Program
{
	friend class ProgramFactory;
private:
	//禁止复制
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

public:
	//统一构造器
	Program(const std::map<ShaderType, std::string>& shadersSource, bool isFromFile = true);
	//顶点，片段程序
	Program(const std::string& vertexPath, const std::string& fragmentPath, bool isFromFile = true);
	//顶点，片段，几何程序
	Program(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, bool isFromFile = true);

public:
	virtual ~Program();

public:
	void Use()const;
	void Link()const;
	const std::string& GetName()const { return m_Name; }
	void SetName(const std::string& name) { this->m_Name = name; }

	void SetUniform(const std::string &uniformName, bool value) const;
	void SetUniform(const std::string &uniformName, int value) const;
	void SetUniform(const std::string &uniformName, float value) const;
	void SetUniform(const std::string &uniformName, const glm::vec2 &value) const;
	void SetUniform(const std::string &uniformName, float x, float y) const;
	void SetUniform(const std::string &uniformName, const glm::vec3 &value) const;
	void SetUniform(const std::string &uniformName, float x, float y, float z) const;
	void SetUniform(const std::string &uniformName, const glm::vec4 &value) const;
	void SetUniform(const std::string &uniformName, float x, float y, float z, float w) const;
	void SetUniform(const std::string &uniformName, const glm::mat2 &mat) const;
	void SetUniform(const std::string &uniformName, const glm::mat3 &mat) const;
	void SetUniform(const std::string &uniformName, const glm::mat4 &mat) const;
	void SetUniform(const std::string &uniformName, Texture1D* pTexture)const;
	void SetUniform(const std::string &uniformName, Texture2D* pTexture)const;

    void SetUniform(const std::string &uniformName, const std::vector<bool> &values) const;
    void SetUniform(const std::string &uniformName, const std::vector<int> &values) const;
    void SetUniform(const std::string &uniformName, const std::vector<float> &values) const;
    void SetUniform(const std::string &uniformName, const std::vector<glm::vec2> &values) const;
    void SetUniform(const std::string &uniformName, const std::vector<glm::vec3> &values) const;
    void SetUniform(const std::string &uniformName, const std::vector<glm::vec4> &values) const;

    bool GetUniform(const std::string &uniformName, bool& value, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, int& value, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, float& value, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, glm::vec2 &value, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, float& x, float& y, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, glm::vec3 &value, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, float& x, float& y, float& z, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, glm::vec4 &value, bool checkSize = false) const;
    bool GetUniform(const std::string &uniformName, float& x, float& y, float& z, float& w, bool checkSize = false) const;

private:
	//检查器编译错误
	bool CheckShaderCompileError(unsigned int shaderId, const ShaderType& shaderType);
	//检查着色器程序连接错误
	bool CheckProgramLinkError();

private:
	unsigned int m_ID;
	std::string m_Name;
};

class ProgramFactory
{
	ProgramFactory() = delete;
	ProgramFactory& operator=(const ProgramFactory&) = delete;
	ProgramFactory(const ProgramFactory&) = delete;
	~ProgramFactory() = delete;

public:
	static bool Init();
	static Program* Get(ProgramType programType);
private:
	static std::map<ProgramType, std::unique_ptr<Program>> m_Programs;
	static bool m_Inited;
};
