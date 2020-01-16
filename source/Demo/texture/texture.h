#pragma once
#include <string>
#include <memory>

class Program;

class Texture
{
private:
	//禁止复制
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture() = default;

public:
	virtual ~Texture();

	static std::unique_ptr<Texture> Create(const std::string& fileName, int textureUnit = 0);

	unsigned int GetID()const { return m_ID; }
	int GetTextureUnit()const { return m_TextureUnit; }
	const std::string& GetFileName()const { return m_FileName; }
	int GetWidth()const { return m_Width; }
	int GetHeight()const { return m_Height; }

private:
	unsigned int m_ID;
	int m_TextureUnit;		//从0开始计数
	std::string m_FileName;
	int m_Width;
	int m_Height;
	int m_Channels;
};