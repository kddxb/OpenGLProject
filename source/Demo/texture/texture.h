#pragma once
#include <string>
#include <memory>

class Texture1D
{
private:
	//��ֹ����
	Texture1D(const Texture1D&) = delete;
	Texture1D& operator=(const Texture1D&) = delete;
	Texture1D() = default;

public:
	virtual ~Texture1D();

	static std::unique_ptr<Texture1D> Create(unsigned char* data, int width, int channels, int initTextureUnit = 0);

	unsigned int GetID()const { return m_ID; }
	int GetTextureUnit()const { return m_TextureUnit; }
	void SetTextUnit(int textureUnit);
	int GetWidth()const { return m_Width; }

private:
	unsigned int m_ID;
	int m_TextureUnit;		//��0��ʼ����
	int m_Width;
	int m_Channels;
};

class Texture2D
{
private:
	//��ֹ����
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	Texture2D() = default;

public:
	virtual ~Texture2D();

	static std::unique_ptr<Texture2D> Create(const std::string& fileName, int initTextureUnit = 0);
	static std::unique_ptr<Texture2D> Create(unsigned char* data, int width, int height, int channels, int initTextureUnit = 0);

	unsigned int GetID()const { return m_ID; }
	int GetTextureUnit()const { return m_TextureUnit; }
    void SetTextUnit(int textureUnit);
	const std::string& GetFileName()const { return m_FileName; }
	int GetWidth()const { return m_Width; }
	int GetHeight()const { return m_Height; }

private:
	unsigned int m_ID;
	int m_TextureUnit;		//��0��ʼ����
	std::string m_FileName;
	int m_Width;
	int m_Height;
	int m_Channels;
};

class TextureUtils
{
private:
    //��ֹ����
    TextureUtils(const TextureUtils&) = delete;
    TextureUtils& operator=(const TextureUtils&) = delete;
    TextureUtils() = delete;

public:
    //��ȡ
    static int GetActiveTextureUnit();
    static void ActiveTextureUnit(int textureUnit);
};