#include "StdAfx.h"
#include "texture.h"
#include <GLAD/glad.h>
#include <stb_image.h>
#include "fileTool.h"
#include "Log.h"
#include "Program.h"

Texture::~Texture()
{
    if (glIsTexture(m_ID))
    {
        glDeleteTextures(1, &m_ID);
    }
}

std::unique_ptr<Texture> Texture::Create(const std::string & fileName, int initTextureUnit)
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

    unsigned int textureID;
    glGenTextures(1, &textureID);
    int oldTextureUnit = TextureUtils::GetActiveTextureUnit();
    TextureUtils::ActiveTextureUnit(initTextureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (nrChannels == 1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    TextureUtils::ActiveTextureUnit(oldTextureUnit);

    std::unique_ptr<Texture> upTexture(new Texture());
    upTexture->m_ID = textureID;
    upTexture->m_TextureUnit = initTextureUnit;
    upTexture->m_FileName = fileName;
    upTexture->m_Width = width;
    upTexture->m_Height = height;
    upTexture->m_Channels = nrChannels;

    return upTexture;
}

void Texture::SetTextUnit(int textureUnit)
{
    if (textureUnit == m_TextureUnit)
    {
        return;
    }
    TextureUtils::ActiveTextureUnit(textureUnit);
    glBindTexture(GL_TEXTURE_2D, this->m_ID);
}

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