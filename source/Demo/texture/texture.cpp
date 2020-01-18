#include "StdAfx.h"
#include "texture.h"
#include <GLAD/glad.h>
#include <stb_image.h>
#include "fileTool.h"
#include "Log.h"
#include "Program.h"

//////////////////////////////////////////////////////////////////////////
// Texture1D
//////////////////////////////////////////////////////////////////////////
Texture1D::~Texture1D()
{
	if (glIsTexture(m_ID))
	{
		glDeleteTextures(1, &m_ID);
	}
}

std::unique_ptr<Texture1D> Texture1D::Create(unsigned char * data, int width, int channels, int initTextureUnit)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int oldTextureUnit = TextureUtils::GetActiveTextureUnit();
	TextureUtils::ActiveTextureUnit(initTextureUnit);
	glBindTexture(GL_TEXTURE_1D, textureID);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ((channels*width) % 4)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	if (channels == 1)
	{
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, width, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	else if (channels == 3)
	{
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (channels == 4)
	{
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glGenerateMipmap(GL_TEXTURE_1D);

	TextureUtils::ActiveTextureUnit(oldTextureUnit);

	std::unique_ptr<Texture1D> upTexture(new Texture1D());
	upTexture->m_ID = textureID;
	upTexture->m_TextureUnit = initTextureUnit;
	upTexture->m_Width = width;
	upTexture->m_Channels = channels;

	return upTexture;
}

void Texture1D::SetTextUnit(int textureUnit)
{
	if (textureUnit == m_TextureUnit)
	{
		return;
	}
	TextureUtils::ActiveTextureUnit(textureUnit);
	glBindTexture(GL_TEXTURE_1D, this->m_ID);
}


//////////////////////////////////////////////////////////////////////////
// Texture2D
//////////////////////////////////////////////////////////////////////////
Texture2D::~Texture2D()
{
    if (glIsTexture(m_ID))
    {
        glDeleteTextures(1, &m_ID);
    }
}

std::unique_ptr<Texture2D> Texture2D::Create(const std::string & fileName, int initTextureUnit)
{
    if (!FileTool::IsFileExist(fileName))
    {
        Log::GetInstance()->Write(fileName + "≤ª¥Ê‘⁄", LogLevel::Error);
        return nullptr;
    }
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        Log::GetInstance()->Write("∂¡»°Õº∆¨" + fileName + " ß∞‹", LogLevel::Error);
        return nullptr;
    }
	std::unique_ptr<Texture2D> upTexture = Create(data, width, height, nrChannels, initTextureUnit);
	upTexture->m_FileName = fileName;
	return std::move(upTexture);
}

std::unique_ptr<Texture2D> Texture2D::Create(unsigned char * data, int width, int height, int channels, int initTextureUnit)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int oldTextureUnit = TextureUtils::GetActiveTextureUnit();
	TextureUtils::ActiveTextureUnit(initTextureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ((channels*width) % 4)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	if (channels == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	else if (channels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (channels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	TextureUtils::ActiveTextureUnit(oldTextureUnit);

	std::unique_ptr<Texture2D> upTexture(new Texture2D());
	upTexture->m_ID = textureID;
	upTexture->m_TextureUnit = initTextureUnit;
	upTexture->m_Width = width;
	upTexture->m_Height = height;
	upTexture->m_Channels = channels;

	return upTexture;
}

void Texture2D::SetTextUnit(int textureUnit)
{
    if (textureUnit == m_TextureUnit)
    {
        return;
    }
    TextureUtils::ActiveTextureUnit(textureUnit);
    glBindTexture(GL_TEXTURE_2D, this->m_ID);
}


//////////////////////////////////////////////////////////////////////////
// TextureUtils
//////////////////////////////////////////////////////////////////////////
int TextureUtils::GetActiveTextureUnit()
{
    int activeTextureUnit;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTextureUnit);
    return activeTextureUnit - GL_TEXTURE0;
}

void TextureUtils::ActiveTextureUnit(int textureUnit)
{
    int activeTextureUnit = TextureUtils::GetActiveTextureUnit();
    if (activeTextureUnit == textureUnit)
    {
        return;
    }
    glActiveTexture(GL_TEXTURE0 + textureUnit);
}