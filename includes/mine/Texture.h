#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "../GL/glad.h"
#include <string>
#include <vector>

class Texture
{
    unsigned int textureID;
    public:
    std::string type;

    Texture();

    bool loadTexture(const char *path, GLenum format = GL_RGBA, GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT, GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR);

    bool loadCubemap(std::vector<std::string> faces);

    void unbind() const;

    unsigned int getTextureID() const;

   // ~Texture();

    friend class TextureManager;
};

#endif // __TEXTURE_H__