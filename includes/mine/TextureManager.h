#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
#include "Texture.h"
#include <unordered_map>

class TextureManager
{
     int textureCount;

     std::unordered_map<unsigned int, int> texturesIDs;

     std::unordered_map<std::string, unsigned int> existingTextures; 
   public:
   TextureManager();
   
    int getTexCount();

    Texture generateTexture();

    void bindTexture(const Texture& texture);

    void bindCubemap(const Texture& texture);

    void loadTexture(const Texture& texture, const char* texturePath);

    bool isTexture(const char* texturePath) const;

    ~TextureManager();
};

extern TextureManager textureManager;

#endif // __TEXTUREMANAGER_H__