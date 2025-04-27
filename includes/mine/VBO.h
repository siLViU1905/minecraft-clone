#ifndef __VBO_H__
#define __VBO_H__
#include "../GL/glad.h"

class VBO
{
   unsigned int vbo;
   public:
   VBO();

   void bind() const;

   void setData(const void* data, size_t size, unsigned int drawType = GL_STATIC_DRAW) const;

   ~VBO();
};

#endif // __VBO_H__