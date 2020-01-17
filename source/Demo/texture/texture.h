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

	static std::unique_ptr<Texture> Create(const std::string& fileName, int initTextureUnit = 0);

	unsigned int GetID()const { return m_ID; }
	int GetTextureUnit()const { return m_TextureUnit; }
    void SetTextUnit(int textureUnit);
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


class TextureUtils
{
private:
    //禁止复制
    TextureUtils(const TextureUtils&) = delete;
    TextureUtils& operator=(const TextureUtils&) = delete;
    TextureUtils() = delete;

public:
    //获取
    static int GetActiveTextureUnit();
    static void ActiveTextureUnit(int textureUnit);
};