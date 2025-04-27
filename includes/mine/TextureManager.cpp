#include "TextureManager.h"

int TextureManager::getTexCount()
{
    return textureCount;
}

Texture TextureManager::generateTexture()
{
    Texture texture;
    glGenTextures(1, &texture.textureID);
    texturesIDs.emplace(texture.textureID, textureCount);
    ++textureCount;
    return texture;
}

void TextureManager::bindTexture(const Texture& texture)
{
    glActiveTexture(GL_TEXTURE0 + texturesIDs[texture.textureID]);
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
}

void TextureManager::bindCubemap(const Texture& texture)
{
    glActiveTexture(GL_TEXTURE0 + texturesIDs[texture.textureID]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.textureID);
}

bool TextureManager::isTexture(const char* texturePath) const
{
    return existingTextures.at(texturePath) != 0;
}

TextureManager::~TextureManager()
{
    for(const auto& pair : texturesIDs)
      glDeleteTextures(1, &pair.first);
}

TextureManager::TextureManager()
{
    textureCount = 0;
}

TextureManager textureManager;
