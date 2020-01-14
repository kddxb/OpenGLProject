#pragma once
#include <vector>
#include <map>
#include <memory>
#include <GLM/glm.hpp>

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

class Program
{
	friend class ProgramFactory;
private:
	//��ֹ����
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

public:
	//ͳһ������
	Program(const std::map<ShaderType, std::string>& shadersSource, bool isFromFile = true);
	//���㣬Ƭ�γ���
	Program(const std::string& vertexPath, const std::string& fragmentPath, bool isFromFile = true);
	//���㣬Ƭ�Σ����γ���
	Program(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, bool isFromFile = true);

public:
	virtual ~Program();

public:
	void Use()const;
	void Link()const;
	const std::string& GetName()const { return m_Name; }
	void SetName(const std::string& name) { this->m_Name = name; }

	void SetUniform(const std::string &name, bool value) const;
	void SetUniform(const std::string &name, int value) const;
	void SetUniform(const std::string &name, float value) const;
	void SetUniform(const std::string &name, const glm::vec2 &value) const;
	void SetUniform(const std::string &name, float x, float y) const;
	void SetUniform(const std::string &name, const glm::vec3 &value) const;
	void SetUniform(const std::string &name, float x, float y, float z) const;
	void SetUniform(const std::string &name, const glm::vec4 &value) const;
	void SetUniform(const std::string &name, float x, float y, float z, float w) const;
	void SetUniform(const std::string &name, const glm::mat2 &mat) const;
	void SetUniform(const std::string &name, const glm::mat3 &mat) const;
	void SetUniform(const std::string &name, const glm::mat4 &mat) const;

private:
	//������������
	bool CheckShaderCompileError(unsigned int shaderId, const ShaderType& shaderType);
	//�����ɫ���������Ӵ���
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
